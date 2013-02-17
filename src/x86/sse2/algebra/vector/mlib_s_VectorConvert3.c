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

#pragma ident	"@(#)mlib_s_VectorConvert3.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorConvert_S8_U8_Sat     - packing of 8-bit format vector
 *      mlib_VectorConvert_U8_S8_Sat     - packing of 8-bit format vector
 *      mlib_VectorConvert_U8_S16_Sat    - packing of 16-bit
 *                                         into 8-bit format vector
 *      mlib_VectorConvert_S8_S16_Sat    - packing of 16-bit
 *                                         into 8-bit format vector
 *      mlib_VectorConvert_U8_S32_Sat    - packing of 32-bit into 8-bit
 *                                         format vector
 *      mlib_VectorConvert_S8_S32_Sat    - packing of 32-bit into 8-bit
 *                                         format vector
 *      mlib_VectorConvert_S16_S32_Sat   - packing of 32-bit into 16-bit
 *                                         format vector
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorConvert_S8_U8_Sat(mlib_s8      *z,
 *                                              const mlib_u8 *x,
 *                                              mlib_s32      n);
 *      mlib_status mlib_VectorConvert_U8_S8_Sat(mlib_u8      *z,
 *                                              const mlib_s8 *x,
 *                                              mlib_s32      n);
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
 *                                                const mlib_u8 *x,
 *                                                mlib_s32      n);
 *      mlib_status mlib_VectorConvert_U8C_S8C_Sat(mlib_u8       *z,
 *                                                const mlib_s8 *x,
 *                                                mlib_s32      n);
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

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

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

#define	SAT_U8(x)		\
	if (x < 0)		\
	    x = 0;		\
	else if (x > 255)	\
	    x = 255

#define	SAT_S8(x)		\
	if (x < -128)		\
	    x = -128;		\
	else if (x > 127)	\
	    x = 127

#define	SAT_S16(x)			\
	if (x < MLIB_S16_MIN)		\
	    x = MLIB_S16_MIN;		\
	else if (x > MLIB_S16_MAX)	\
	    x = MLIB_S16_MAX

/* ****************************************************************** */

mlib_status
__mlib_VectorConvert_S8_U8_Sat(
	mlib_s8 *z,
	const mlib_u8 *x,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, ax, az, nstep, n1, n2, n3, xval;
	mlib_u8 *px = (mlib_u8 *)x;
	mlib_s8 *pz = (mlib_s8 *)z;
	__m128i zbuf, xbuf, mask;
	mask = _mm_set1_epi8(127);

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_u8);
	n1 = ((16 - ax) & 15) / sizeof (mlib_u8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			xval = *px++;
			if (xval > 127)
				xval = 127;
			*pz++ = xval;
		}
	} else {
		for (i = 0; i < n1; i++) {
			xval = *px++;
			if (xval > 127)
				xval = 127;
			*pz++ = xval;
		}

		for (i = 0; i < n2; i++) {
			xbuf = _mm_load_si128((__m128i *)px);
			zbuf = _mm_min_epu8(xbuf, mask);
			_mm_storeu_si128((__m128i *)pz, zbuf);
			px += nstep;
			pz += nstep;
		}

		for (i = 0; i < n3; i++) {
			xval = *px++;
			if (xval > 127)
				xval = 127;
			*pz++ = xval;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_U8_S8_Sat(
	mlib_u8 *z,
	const mlib_s8 *x,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, ax, az, nstep, n1, n2, n3, xval;
	mlib_s8 *px = (mlib_s8 *)x;
	mlib_u8 *pz = (mlib_u8 *)z;
	__m128i zbuf, xbuf, zero, mask;
	zero = _mm_setzero_si128();

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_u8);
	n1 = ((16 - ax) & 15) / sizeof (mlib_u8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			xval = *px++;
			if (xval < 0)
				xval = 0;
			*pz++ = xval;
		}
	} else {
		for (i = 0; i < n1; i++) {
			xval = *px++;
			if (xval < 0)
				xval = 0;
			*pz++ = xval;
		}

		for (i = 0; i < n2; i++) {
			xbuf = _mm_load_si128((__m128i *)px);
			mask = _mm_cmpgt_epi8(zero, xbuf);
			zbuf = _mm_andnot_si128(mask, xbuf);
			_mm_storeu_si128((__m128i *)pz, zbuf);
			px += nstep;
			pz += nstep;
		}

		for (i = 0; i < n3; i++) {
			xval = *px++;
			if (xval < 0)
				xval = 0;
			*pz++ = xval;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_U8_S16_Sat(
	mlib_u8 *z,
	const mlib_s16 *x,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, ax, az, nstep, n1, n2, n3, xval;
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
			xval = *px++;
			SAT_U8(xval);
			*pz++ = xval;
		}
	} else {
		for (i = 0; i < n1; i++) {
			xval = *px++;
			SAT_U8(xval);
			*pz++ = xval;
		}

		for (i = 0; i < n2; i++) {
			xlo = _mm_load_si128((__m128i *)px);
			xhi = _mm_load_si128((__m128i *)px + 1);
			zbuf = _mm_packus_epi16(xlo, xhi);
			_mm_storeu_si128((__m128i *)pz, zbuf);
			px += nstep;
			pz += nstep;
		}

		for (i = 0; i < n3; i++) {
			xval = *px++;
			SAT_U8(xval);
			*pz++ = xval;
		}
	}

	return (MLIB_SUCCESS);
}
/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S8_S16_Sat(
	mlib_s8 *z,
	const mlib_s16 *x,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, ax, az, nstep, n1, n2, n3, xval;
	mlib_s16 *px = (mlib_s16 *)x;
	mlib_s8 *pz = (mlib_s8 *)z;
	__m128i zbuf, xlo, xhi, mask;

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_s8);
	n1 = ((16 - ax) & 15) / sizeof (mlib_s16);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			xval = *px++;
			SAT_S8(xval);
			*pz++ = xval;
		}
	} else {
		for (i = 0; i < n1; i++) {
			xval = *px++;
			SAT_S8(xval);
			*pz++ = xval;
		}

		for (i = 0; i < n2; i++) {
			xlo = _mm_load_si128((__m128i *)px);
			xhi = _mm_load_si128((__m128i *)px + 1);
			zbuf = _mm_packs_epi16(xlo, xhi);
			_mm_storeu_si128((__m128i *)pz, zbuf);
			px += nstep;
			pz += nstep;
		}

		for (i = 0; i < n3; i++) {
			xval = *px++;
			SAT_S8(xval);
			*pz++ = xval;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_U8_S32_Sat(
	mlib_u8 *z,
	const mlib_s32 *x,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, ax, az, nstep, n1, n2, n3, xval;
	mlib_s32 *px = (mlib_s32 *)x;
	mlib_u8 *pz = (mlib_u8 *)z;
	__m128i zbuf, buf1, buf2, buf3, buf4, mask;

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_u8);
	n1 = ((16 - ax) & 15) / sizeof (mlib_s32);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			xval = *px++;
			SAT_U8(xval);
			*pz++ = xval;
		}
	} else {
		for (i = 0; i < n1; i++) {
			xval = *px++;
			SAT_U8(xval);
			*pz++ = xval;
		}

		for (i = 0; i < n2; i++) {
			buf1 = _mm_load_si128((__m128i *)px);
			buf2 = _mm_load_si128((__m128i *)px + 1);
			buf3 = _mm_load_si128((__m128i *)px + 2);
			buf4 = _mm_load_si128((__m128i *)px + 3);
			buf1 = _mm_packs_epi32(buf1, buf2);
			buf3 = _mm_packs_epi32(buf3, buf4);
			zbuf = _mm_packus_epi16(buf1, buf3);
			_mm_storeu_si128((__m128i *)pz, zbuf);
			px += nstep;
			pz += nstep;
		}

		for (i = 0; i < n3; i++) {
			xval = *px++;
			SAT_U8(xval);
			*pz++ = xval;
		}
	}

	return (MLIB_SUCCESS);
}
/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S8_S32_Sat(
	mlib_s8 *z,
	const mlib_s32 *x,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, ax, az, nstep, n1, n2, n3, xval;
	mlib_s32 *px = (mlib_s32 *)x;
	mlib_s8 *pz = (mlib_s8 *)z;
	__m128i zbuf, buf1, buf2, buf3, buf4, mask;

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_s8);
	n1 = ((16 - ax) & 15) / sizeof (mlib_s32);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			xval = *px++;
			SAT_S8(xval);
			*pz++ = xval;
		}
	} else {
		for (i = 0; i < n1; i++) {
			xval = *px++;
			SAT_S8(xval);
			*pz++ = xval;
		}

		for (i = 0; i < n2; i++) {
			buf1 = _mm_load_si128((__m128i *)px);
			buf2 = _mm_load_si128((__m128i *)px + 1);
			buf3 = _mm_load_si128((__m128i *)px + 2);
			buf4 = _mm_load_si128((__m128i *)px + 3);
			buf1 = _mm_packs_epi32(buf1, buf2);
			buf3 = _mm_packs_epi32(buf3, buf4);
			zbuf = _mm_packs_epi16(buf1, buf3);
			_mm_storeu_si128((__m128i *)pz, zbuf);
			px += nstep;
			pz += nstep;
		}

		for (i = 0; i < n3; i++) {
			xval = *px++;
			SAT_S8(xval);
			*pz++ = xval;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S16_S32_Sat(
	mlib_s16 *z,
	const mlib_s32 *x,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, ax, az, nstep, n1, n2, n3, xval;
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
			xval = *px++;
			SAT_S16(xval);
			*pz++ = xval;
		}
	} else {
		for (i = 0; i < n1; i++) {
			xval = *px++;
			SAT_S16(xval);
			*pz++ = xval;
		}

		for (i = 0; i < n2; i++) {
			xlo = _mm_load_si128((__m128i *)px);
			xhi = _mm_load_si128((__m128i *)px + 1);
			zbuf = _mm_packs_epi32(xlo, xhi);
			_mm_storeu_si128((__m128i *)pz, zbuf);
			px += nstep;
			pz += nstep;
		}

		for (i = 0; i < n3; i++) {
			xval = *px++;
			SAT_S16(xval);
			*pz++ = xval;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S8C_U8C_Sat(
	mlib_s8 *z,
	const mlib_u8 *x,
	mlib_s32 n)
{
	return (__mlib_VectorConvert_S8_U8_Sat(z, x, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_U8C_S8C_Sat(
	mlib_u8 *z,
	const mlib_s8 *x,
	mlib_s32 n)
{
	return (__mlib_VectorConvert_U8_S8_Sat(z, x, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_U8C_S16C_Sat(
	mlib_u8 *z,
	const mlib_s16 *x,
	mlib_s32 n)
{
	return (__mlib_VectorConvert_U8_S16_Sat(z, x, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S8C_S16C_Sat(
	mlib_s8 *z,
	const mlib_s16 *x,
	mlib_s32 n)
{
	return (__mlib_VectorConvert_S8_S16_Sat(z, x, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_U8C_S32C_Sat(
	mlib_u8 *z,
	const mlib_s32 *x,
	mlib_s32 n)
{
	return (__mlib_VectorConvert_U8_S32_Sat(z, x, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S8C_S32C_Sat(
	mlib_s8 *z,
	const mlib_s32 *x,
	mlib_s32 n)
{
	return (__mlib_VectorConvert_S8_S32_Sat(z, x, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S16C_S32C_Sat(
	mlib_s16 *z,
	const mlib_s32 *x,
	mlib_s32 n)
{
	return (__mlib_VectorConvert_S16_S32_Sat(z, x, n + n));
}

/* *********************************************************** */
