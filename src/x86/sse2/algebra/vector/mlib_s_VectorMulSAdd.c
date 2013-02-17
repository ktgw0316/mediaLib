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

#pragma ident	"@(#)mlib_s_VectorMulSAdd.c	9.3	07/11/05 SMI"

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

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* ****************************************************************** */

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

/* ****************************************************************** */

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

/* ****************************************************************** */

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

/* ****************************************************************** */

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

#define	SAT_S32(x)			\
	if (x < MLIB_S32_MIN)		\
	    x = MLIB_S32_MIN;		\
	else if (x > MLIB_S32_MAX)	\
	    x = MLIB_S32_MAX

/* ****************************************************************** */

#define	MULS_U8_SAT(ld1, ld2, st)		\
	xbuf = _mm_##ld1##_si128((__m128i *)px);\
	ybuf = _mm_##ld2##_si128((__m128i *)py);\
						\
	xlo = _mm_unpacklo_epi8(xbuf, zero);	\
	xhi = _mm_unpackhi_epi8(xbuf, zero);	\
	ylo = _mm_unpacklo_epi8(ybuf, zero);	\
	yhi = _mm_unpackhi_epi8(ybuf, zero);	\
	ylo = _mm_mullo_epi16(ylo, cbuf);	\
	yhi = _mm_mullo_epi16(yhi, cbuf);	\
	ylo = _mm_max_epi16(ylo, mask1);	\
	yhi = _mm_max_epi16(yhi, mask1);	\
	ylo = _mm_and_si128(ylo, mask2);	\
	yhi = _mm_and_si128(yhi, mask2);	\
	xlo = _mm_adds_epi16(xlo, ylo);		\
	xhi = _mm_adds_epi16(xhi, yhi);		\
	zbuf = _mm_packus_epi16(xlo, xhi);	\
	_mm_##st##_si128((__m128i *)pz, zbuf);	\
	px += nstep;				\
	py += nstep;				\
	pz += nstep

/* ****************************************************************** */

mlib_status
__mlib_VectorMulSAdd_U8_U8_Sat(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	const mlib_u8 *c,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum, c0, ax, ay, az, nstep, n1, n2, n3;
	mlib_u8 *px = (mlib_u8 *)x, *py = (mlib_u8 *)y, *pz = (mlib_u8 *)z;
	__m128i xbuf, ybuf, zbuf, cbuf, xlo, xhi, ylo, yhi;
	__m128i zero, mask1, mask2;

	c0 = c[0];

	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_u8);
	n1 = ((16 - ax) & 15) / sizeof (mlib_u8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = (*px++) + (*py++) * c0;
			SAT_U8(sum);
			*pz++ = sum;
		}
	} else {
		zero = _mm_setzero_si128();
		cbuf = _mm_set1_epi16(c0);
		mask1 = _mm_set1_epi16(-1);
		mask2 = _mm_set1_epi16(32767);

		for (i = 0; i < n1; i++) {
			sum = (*px++) + (*py++) * c0;
			SAT_U8(sum);
			*pz++ = sum;
		}

		if (ax == ay && ax == az) {
			for (i = 0; i < n2; i++) {
				MULS_U8_SAT(load, load, store);
			}
		} else if (ax == ay) {
			for (i = 0; i < n2; i++) {
				MULS_U8_SAT(load, load, storeu);
			}
		} else {
			for (i = 0; i < n2; i++) {
				MULS_U8_SAT(load, loadu, storeu);
			}
		}

		for (i = 0; i < n3; i++) {
			sum = (*px++) + (*py++) * c0;
			SAT_U8(sum);
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
}

/* ****************************************************************** */

#define	MULS_U8_MOD(ld1, ld2, st)		\
	xbuf = _mm_##ld1##_si128((__m128i *)px);\
	ybuf = _mm_##ld2##_si128((__m128i *)py);\
						\
	ylo = _mm_unpacklo_epi8(ybuf, zero);	\
	yhi = _mm_unpackhi_epi8(ybuf, zero);	\
	ylo = _mm_mullo_epi16(ylo, cbuf);	\
	yhi = _mm_mullo_epi16(yhi, cbuf);	\
	ylo = _mm_and_si128(ylo, mask);		\
	yhi = _mm_and_si128(yhi, mask);		\
	zbuf = _mm_packus_epi16(ylo, yhi);	\
	zbuf = _mm_add_epi8(zbuf, xbuf);	\
	_mm_##st##_si128((__m128i *)pz, zbuf);	\
	px += nstep;				\
	py += nstep;				\
	pz += nstep

/* ****************************************************************** */

mlib_status
__mlib_VectorMulSAdd_U8_U8_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	const mlib_u8 *c,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum, c0, ax, ay, az, nstep, n1, n2, n3;
	mlib_u8 *px = (mlib_u8 *)x, *py = (mlib_u8 *)y, *pz = (mlib_u8 *)z;
	__m128i xbuf, ybuf, zbuf, cbuf, xlo, xhi, ylo, yhi;
	__m128i zero, mask;

	c0 = c[0];

	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_u8);
	n1 = ((16 - ax) & 15) / sizeof (mlib_u8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = (*px++) + (*py++) * c0;
			*pz++ = sum;
		}
	} else {
		zero = _mm_setzero_si128();
		cbuf = _mm_set1_epi16(c0);
		mask = _mm_set1_epi16(0xff);

		for (i = 0; i < n1; i++) {
			sum = (*px++) + (*py++) * c0;
			*pz++ = sum;
		}

		if (ax == ay && ax == az) {
			for (i = 0; i < n2; i++) {
				MULS_U8_MOD(load, load, store);
			}
		} else if (ax == ay) {
			for (i = 0; i < n2; i++) {
				MULS_U8_MOD(load, load, storeu);
			}
		} else {
			for (i = 0; i < n2; i++) {
				MULS_U8_MOD(load, loadu, storeu);
			}
		}

		for (i = 0; i < n3; i++) {
			sum = (*px++) + (*py++) * c0;
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
}

/* ****************************************************************** */

#define	MULS_S8_SAT(ld1, ld2, st)		\
	xbuf = _mm_##ld1##_si128((__m128i *)px);\
	ybuf = _mm_##ld2##_si128((__m128i *)py);\
						\
	xlo = _mm_unpacklo_epi8(zero, xbuf);	\
	xhi = _mm_unpackhi_epi8(zero, xbuf);	\
	xlo = _mm_srai_epi16(xlo, 8);		\
	xhi = _mm_srai_epi16(xhi, 8);		\
	ylo = _mm_unpacklo_epi8(zero, ybuf);	\
	yhi = _mm_unpackhi_epi8(zero, ybuf);	\
	ylo = _mm_mulhi_epi16(ylo, cbuf);	\
	yhi = _mm_mulhi_epi16(yhi, cbuf);	\
	xlo = _mm_add_epi16(xlo, ylo);		\
	xhi = _mm_add_epi16(xhi, yhi);		\
	zbuf = _mm_packs_epi16(xlo, xhi);	\
	_mm_##st##_si128((__m128i *)pz, zbuf);	\
	px += nstep;				\
	py += nstep;				\
	pz += nstep

/* ****************************************************************** */

mlib_status
__mlib_VectorMulSAdd_S8_S8_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	const mlib_s8 *c,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum, c0, ax, ay, az, nstep, n1, n2, n3;
	mlib_s8 *px = (mlib_s8 *)x, *py = (mlib_s8 *)y, *pz = (mlib_s8 *)z;
	__m128i xbuf, ybuf, zbuf, cbuf, xlo, xhi, ylo, yhi;
	__m128i zero, mask1, mask2;

	c0 = c[0];

	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_s8);
	n1 = ((16 - ax) & 15) / sizeof (mlib_s8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = (*px++) + (*py++) * c0;
			SAT_S8(sum);
			*pz++ = sum;
		}
	} else {
		zero = _mm_setzero_si128();
		cbuf = _mm_set1_epi16(c0 << 8);

		for (i = 0; i < n1; i++) {
			sum = (*px++) + (*py++) * c0;
			SAT_S8(sum);
			*pz++ = sum;
		}

		if (ax == ay && ax == az) {
			for (i = 0; i < n2; i++) {
				MULS_S8_SAT(load, load, store);
			}
		} else if (ax == ay) {
			for (i = 0; i < n2; i++) {
				MULS_S8_SAT(load, load, storeu);
			}
		} else {
			for (i = 0; i < n2; i++) {
				MULS_S8_SAT(load, loadu, storeu);
			}
		}

		for (i = 0; i < n3; i++) {
			sum = (*px++) + (*py++) * c0;
			SAT_S8(sum);
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
}

/* ****************************************************************** */

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

/* ****************************************************************** */

#define	MULS_S16_SAT(ld1, ld2, st)		\
	xbuf = _mm_##ld1##_si128((__m128i *)px);\
	ybuf = _mm_##ld2##_si128((__m128i *)py);\
						\
	xlo = _mm_unpacklo_epi16(xbuf, ybuf);	\
	xhi = _mm_unpackhi_epi16(xbuf, ybuf);	\
	xlo = _mm_madd_epi16(xlo, cbuf);	\
	xhi = _mm_madd_epi16(xhi, cbuf);	\
	zbuf = _mm_packs_epi32(xlo, xhi);	\
	_mm_##st##_si128((__m128i *)pz, zbuf);	\
	px += nstep;				\
	py += nstep;				\
	pz += nstep

/* ****************************************************************** */

mlib_status
__mlib_VectorMulSAdd_S16_S16_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	const mlib_s16 *c,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum, c0, ax, ay, az, nstep, n1, n2, n3;
	mlib_s16 *px = (mlib_s16 *)x, *py = (mlib_s16 *)y, *pz = (mlib_s16 *)z;
	__m128i xbuf, ybuf, zbuf, cbuf, xlo, xhi, ylo, yhi;
	__m128i zero, mask1, mask2;

	c0 = c[0];

	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_s16);
	n1 = ((16 - ax) & 15) / sizeof (mlib_s16);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = (*px++) + (*py++) * c0;
			SAT_S16(sum);
			*pz++ = sum;
		}
	} else {
		cbuf = _mm_set1_epi32((c0 << 16) | 1);

		for (i = 0; i < n1; i++) {
			sum = (*px++) + (*py++) * c0;
			SAT_S16(sum);
			*pz++ = sum;
		}

		if (ax == ay && ax == az) {
			for (i = 0; i < n2; i++) {
				MULS_S16_SAT(load, load, store);
			}
		} else if (ax == ay) {
			for (i = 0; i < n2; i++) {
				MULS_S16_SAT(load, load, storeu);
			}
		} else {
			for (i = 0; i < n2; i++) {
				MULS_S16_SAT(load, loadu, storeu);
			}
		}

		for (i = 0; i < n3; i++) {
			sum = (*px++) + (*py++) * c0;
			SAT_S16(sum);
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
}

/* ****************************************************************** */

#define	MULS_S16_MOD(ld1, ld2, st)		\
	xbuf = _mm_##ld1##_si128((__m128i *)px);\
	ybuf = _mm_##ld2##_si128((__m128i *)py);\
						\
	zbuf = _mm_mullo_epi16(ybuf, cbuf);	\
	zbuf = _mm_add_epi16(zbuf, xbuf);	\
	_mm_##st##_si128((__m128i *)pz, zbuf);	\
	px += nstep;				\
	py += nstep;				\
	pz += nstep

/* ****************************************************************** */

mlib_status
__mlib_VectorMulSAdd_S16_S16_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	const mlib_s16 *c,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum, c0, ax, ay, az, nstep, n1, n2, n3;
	mlib_s16 *px = (mlib_s16 *)x, *py = (mlib_s16 *)y, *pz = (mlib_s16 *)z;
	__m128i xbuf, ybuf, zbuf, cbuf, xlo, xhi, ylo, yhi;
	__m128i zero, mask1, mask2;

	c0 = c[0];

	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_s16);
	n1 = ((16 - ax) & 15) / sizeof (mlib_s16);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = (*px++) + (*py++) * c0;
			*pz++ = sum;
		}
	} else {
		zero = _mm_setzero_si128();
		cbuf = _mm_set1_epi16(c0);

		for (i = 0; i < n1; i++) {
			sum = (*px++) + (*py++) * c0;
			*pz++ = sum;
		}

		if (ax == ay && ax == az) {
			for (i = 0; i < n2; i++) {
				MULS_S16_MOD(load, load, store);
			}
		} else if (ax == ay) {
			for (i = 0; i < n2; i++) {
				MULS_S16_MOD(load, load, storeu);
			}
		} else {
			for (i = 0; i < n2; i++) {
				MULS_S16_MOD(load, loadu, storeu);
			}
		}

		for (i = 0; i < n3; i++) {
			sum = (*px++) + (*py++) * c0;
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
}

/* ****************************************************************** */

#define	MULS_S16U8_SAT(ld1, ld2, st)			\
	xbuf = _mm_##ld1##_si128((__m128i *)px);	\
	ybuf = _mm_##ld2##_si128((__m128i *)py);	\
							\
	xlo = _mm_unpacklo_epi8(xbuf, zero);		\
	xhi = _mm_unpackhi_epi8(xbuf, zero);		\
	ylo = _mm_unpacklo_epi8(ybuf, zero);		\
	yhi = _mm_unpackhi_epi8(ybuf, zero);		\
	ylo = _mm_mullo_epi16(ylo, cbuf);		\
	yhi = _mm_mullo_epi16(yhi, cbuf);		\
	ylo = _mm_max_epi16(ylo, mask1);		\
	yhi = _mm_max_epi16(yhi, mask1);		\
	ylo = _mm_and_si128(ylo, mask2);		\
	yhi = _mm_and_si128(yhi, mask2);		\
	xlo = _mm_adds_epi16(xlo, ylo);			\
	xhi = _mm_adds_epi16(xhi, yhi);			\
	_mm_##st##_si128((__m128i *)pz, xlo);		\
	_mm_##st##_si128((__m128i *)pz + 1, xhi);	\
	px += nstep;					\
	py += nstep;					\
	pz += nstep

/* ****************************************************************** */

mlib_status
__mlib_VectorMulSAdd_S16_U8_Sat(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	const mlib_u8 *c,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum, c0, ax, ay, az, nstep, n1, n2, n3;
	mlib_u8 *px = (mlib_u8 *)x, *py = (mlib_u8 *)y;
	mlib_s16 *pz = (mlib_s16 *)z;
	__m128i xbuf, ybuf, zbuf, cbuf, xlo, xhi, ylo, yhi;
	__m128i zero, mask1, mask2;

	c0 = c[0];

	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_u8);
	n1 = ((16 - ax) & 15) / sizeof (mlib_u8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = (*px++) + (*py++) * c0;
			SAT_S16(sum);
			*pz++ = sum;
		}
	} else {
		zero = _mm_setzero_si128();
		cbuf = _mm_set1_epi16(c0);
		mask1 = _mm_set1_epi16(-1);
		mask2 = _mm_set1_epi16(32767);

		for (i = 0; i < n1; i++) {
			sum = (*px++) + (*py++) * c0;
			SAT_S16(sum);
			*pz++ = sum;
		}

		if (ax == ay && (ax * 2 & 15) == az) {
			for (i = 0; i < n2; i++) {
				MULS_S16U8_SAT(load, load, store);
			}
		} else if (ax == ay) {
			for (i = 0; i < n2; i++) {
				MULS_S16U8_SAT(load, load, storeu);
			}
		} else {
			for (i = 0; i < n2; i++) {
				MULS_S16U8_SAT(load, loadu, storeu);
			}
		}

		for (i = 0; i < n3; i++) {
			sum = (*px++) + (*py++) * c0;
			SAT_S16(sum);
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
}

/* ****************************************************************** */

#define	MULS_S16U8_MOD(ld1, ld2, st)			\
	xbuf = _mm_##ld1##_si128((__m128i *)px);	\
	ybuf = _mm_##ld2##_si128((__m128i *)py);	\
							\
	xlo = _mm_unpacklo_epi8(xbuf, zero);		\
	xhi = _mm_unpackhi_epi8(xbuf, zero);		\
	ylo = _mm_unpacklo_epi8(ybuf, zero);		\
	yhi = _mm_unpackhi_epi8(ybuf, zero);		\
	ylo = _mm_mullo_epi16(ylo, cbuf);		\
	yhi = _mm_mullo_epi16(yhi, cbuf);		\
	xlo = _mm_add_epi16(xlo, ylo);			\
	xhi = _mm_add_epi16(xhi, yhi);			\
	_mm_##st##_si128((__m128i *)pz, xlo);		\
	_mm_##st##_si128((__m128i *)pz + 1, xhi);	\
	px += nstep;					\
	py += nstep;					\
	pz += nstep

/* ****************************************************************** */

mlib_status
__mlib_VectorMulSAdd_S16_U8_Mod(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	const mlib_u8 *c,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum, c0, ax, ay, az, nstep, n1, n2, n3;
	mlib_u8 *px = (mlib_u8 *)x, *py = (mlib_u8 *)y;
	mlib_s16 *pz = (mlib_s16 *)z;
	__m128i xbuf, ybuf, zbuf, cbuf, xlo, xhi, ylo, yhi;
	__m128i zero, mask1, mask2;

	c0 = c[0];

	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_u8);
	n1 = ((16 - ax) & 15) / sizeof (mlib_u8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = (*px++) + (*py++) * c0;
			*pz++ = sum;
		}
	} else {
		zero = _mm_setzero_si128();
		cbuf = _mm_set1_epi16(c0);

		for (i = 0; i < n1; i++) {
			sum = (*px++) + (*py++) * c0;
			*pz++ = sum;
		}

		if (ax == ay && (ax * 2 & 15) == az) {
			for (i = 0; i < n2; i++) {
				MULS_S16U8_MOD(load, load, store);
			}
		} else if (ax == ay) {
			for (i = 0; i < n2; i++) {
				MULS_S16U8_MOD(load, load, storeu);
			}
		} else {
			for (i = 0; i < n2; i++) {
				MULS_S16U8_MOD(load, loadu, storeu);
			}
		}

		for (i = 0; i < n3; i++) {
			sum = (*px++) + (*py++) * c0;
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
}

/* ****************************************************************** */

#define	MULS_S16S8_MOD(ld1, ld2, st)			\
	xbuf = _mm_##ld1##_si128((__m128i *)px);	\
	ybuf = _mm_##ld2##_si128((__m128i *)py);	\
							\
	xlo = _mm_unpacklo_epi8(zero, xbuf);		\
	xhi = _mm_unpackhi_epi8(zero, xbuf);		\
	xlo = _mm_srai_epi16(xlo, 8);			\
	xhi = _mm_srai_epi16(xhi, 8);			\
	ylo = _mm_unpacklo_epi8(zero, ybuf);		\
	yhi = _mm_unpackhi_epi8(zero, ybuf);		\
	ylo = _mm_mulhi_epi16(ylo, cbuf);		\
	yhi = _mm_mulhi_epi16(yhi, cbuf);		\
	xlo = _mm_add_epi16(xlo, ylo);			\
	xhi = _mm_add_epi16(xhi, yhi);			\
	_mm_##st##_si128((__m128i *)pz, xlo);		\
	_mm_##st##_si128((__m128i *)pz + 1, xhi);	\
	px += nstep;					\
	py += nstep;					\
	pz += nstep

/* ****************************************************************** */

mlib_status
__mlib_VectorMulSAdd_S16_S8_Mod(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	const mlib_s8 *c,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum, c0, ax, ay, az, nstep, n1, n2, n3;
	mlib_s8 *px = (mlib_s8 *)x, *py = (mlib_s8 *)y;
	mlib_s16 *pz = (mlib_s16 *)z;
	__m128i xbuf, ybuf, zbuf, cbuf, xlo, xhi, ylo, yhi;
	__m128i zero, mask1, mask2;

	c0 = c[0];

	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_s8);
	n1 = ((16 - ax) & 15) / sizeof (mlib_s8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = (*px++) + (*py++) * c0;
			*pz++ = sum;
		}
	} else {
		zero = _mm_setzero_si128();
		cbuf = _mm_set1_epi16(c0 << 8);

		for (i = 0; i < n1; i++) {
			sum = (*px++) + (*py++) * c0;
			*pz++ = sum;
		}

		if (ax == ay && (ax * 2 & 15) == az) {
			for (i = 0; i < n2; i++) {
				MULS_S16S8_MOD(load, load, store);
			}
		} else if (ax == ay) {
			for (i = 0; i < n2; i++) {
				MULS_S16S8_MOD(load, load, storeu);
			}
		} else {
			for (i = 0; i < n2; i++) {
				MULS_S16S8_MOD(load, loadu, storeu);
			}
		}

		for (i = 0; i < n3; i++) {
			sum = (*px++) + (*py++) * c0;
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
}

/* ****************************************************************** */

#define	MULS_S32S16_MOD(ld1, ld2, st)			\
	xbuf = _mm_##ld1##_si128((__m128i *)px);	\
	ybuf = _mm_##ld2##_si128((__m128i *)py);	\
							\
	xlo = _mm_unpacklo_epi16(xbuf, ybuf);		\
	xhi = _mm_unpackhi_epi16(xbuf, ybuf);		\
	xlo = _mm_madd_epi16(xlo, cbuf);		\
	xhi = _mm_madd_epi16(xhi, cbuf);		\
	_mm_##st##_si128((__m128i *)pz, xlo);		\
	_mm_##st##_si128((__m128i *)pz + 1, xhi);	\
	px += nstep;					\
	py += nstep;					\
	pz += nstep

/* ****************************************************************** */

mlib_status
__mlib_VectorMulSAdd_S32_S16_Mod(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	const mlib_s16 *c,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum, c0, ax, ay, az, nstep, n1, n2, n3;
	mlib_s16 *px = (mlib_s16 *)x, *py = (mlib_s16 *)y;
	mlib_s32 *pz = (mlib_s32 *)z;
	__m128i xbuf, ybuf, zbuf, cbuf, xlo, xhi, ylo, yhi;
	__m128i zero, mask1, mask2;

	c0 = c[0];

	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_s16);
	n1 = ((16 - ax) & 15) / sizeof (mlib_s16);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = (*px++) + (*py++) * c0;
			*pz++ = sum;
		}
	} else {
		cbuf = _mm_set1_epi32((c0 << 16) | 1);

		for (i = 0; i < n1; i++) {
			sum = (*px++) + (*py++) * c0;
			*pz++ = sum;
		}

		if (ax == ay && (ax * 2 & 15) == az) {
			for (i = 0; i < n2; i++) {
				MULS_S32S16_MOD(load, load, store);
			}
		} else if (ax == ay) {
			for (i = 0; i < n2; i++) {
				MULS_S32S16_MOD(load, load, storeu);
			}
		} else {
			for (i = 0; i < n2; i++) {
				MULS_S32S16_MOD(load, loadu, storeu);
			}
		}

		for (i = 0; i < n3; i++) {
			sum = (*px++) + (*py++) * c0;
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
}

/* ****************************************************************** */

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

/* ****************************************************************** */

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

/* ****************************************************************** */

mlib_status
__mlib_VectorMulSAdd_U8_Sat(
	mlib_u8 *xz,
	const mlib_u8 *y,
	const mlib_u8 *c,
	mlib_s32 n)
{
	return (__mlib_VectorMulSAdd_U8_U8_Sat(xz, xz, y, c, n));
}

/* ****************************************************************** */

mlib_status
__mlib_VectorMulSAdd_U8_Mod(
	mlib_u8 *xz,
	const mlib_u8 *y,
	const mlib_u8 *c,
	mlib_s32 n)
{
	return (__mlib_VectorMulSAdd_U8_U8_Mod(xz, xz, y, c, n));
}

/* ****************************************************************** */

mlib_status
__mlib_VectorMulSAdd_S8_Sat(
	mlib_s8 *xz,
	const mlib_s8 *y,
	const mlib_s8 *c,
	mlib_s32 n)
{
	return (__mlib_VectorMulSAdd_S8_S8_Sat(xz, xz, y, c, n));
}

/* ****************************************************************** */

mlib_status
__mlib_VectorMulSAdd_S8_Mod(
	mlib_s8 *xz,
	const mlib_s8 *y,
	const mlib_s8 *c,
	mlib_s32 n)
{
	return (__mlib_VectorMulSAdd_S8_S8_Mod(xz, xz, y, c, n));
}

/* ****************************************************************** */

mlib_status
__mlib_VectorMulSAdd_S16_Sat(
	mlib_s16 *xz,
	const mlib_s16 *y,
	const mlib_s16 *c,
	mlib_s32 n)
{
	return (__mlib_VectorMulSAdd_S16_S16_Sat(xz, xz, y, c, n));
}

/* ****************************************************************** */

mlib_status
__mlib_VectorMulSAdd_S16_Mod(
	mlib_s16 *xz,
	const mlib_s16 *y,
	const mlib_s16 *c,
	mlib_s32 n)
{
	return (__mlib_VectorMulSAdd_S16_S16_Mod(xz, xz, y, c, n));
}

/* ****************************************************************** */

mlib_status
__mlib_VectorMulSAdd_S32_Sat(
	mlib_s32 *xz,
	const mlib_s32 *y,
	const mlib_s32 *c,
	mlib_s32 n)
{
	return (__mlib_VectorMulSAdd_S32_S32_Sat(xz, xz, y, c, n));
}

/* ****************************************************************** */

mlib_status
__mlib_VectorMulSAdd_S32_Mod(
	mlib_s32 *xz,
	const mlib_s32 *y,
	const mlib_s32 *c,
	mlib_s32 n)
{
	return (__mlib_VectorMulSAdd_S32_S32_Mod(xz, xz, y, c, n));
}

/* ****************************************************************** */
