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

#pragma ident	"@(#)mlib_s_VolumeWindowLevel.c	9.5	07/11/05 SMI"

/*
 *  FUNCTION
 *    mlib_VolumeWindowLevel
 *
 *  SYNOPSIS
 *    mlib_status mlib_VolumeWindowLevel(mlib_u8        *dst,
 *                                       const mlib_s16 *src,
 *                                       mlib_s32       window,
 *                                       mlib_s32       level,
 *                                       mlib_s32       gmax,
 *                                       mlib_s32       gmin,
 *                                       mlib_s32       len)
 *
 *  ARGUMENTS
 *    dst       Pointer to output or destination array.
 *    src       Pointer to input or source array.
 *    window    Width of window.
 *    level     Center of window.
 *    gmax      Maximum grayscale in the destination array.
 *    gmin      Minimum grayscale in the destination array.
 *    len       Length of data array.
 *
 *  DESCRIPTION
 *
 */

#include <stdlib.h>
#include <mlib_volume.h>
#ifdef	__SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VolumeWindowLevel = __mlib_VolumeWindowLevel

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VolumeWindowLevel) mlib_VolumeWindowLevel
	__attribute__((weak, alias("__mlib_VolumeWindowLevel")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	_mm_cmplt_epi16(a, b)	_mm_cmpgt_epi16(b, a)

/* *********************************************************** */

#define	INIT_VARS                                                          \
	mlib_s32 scale = 0;                                                \
	mlib_d64 a = (mlib_d64)(gmax - gmin) / window;                     \
	mlib_d64 a_s = a;                                                  \
	mlib_d64 b_s = gmin - a_s * (level - window * 0.5);                \
	mlib_s32 rsl, s;                                                   \
	mlib_s32 hmask, lmask;                                             \
	mlib_s32 ai = (int)(a * (1 << 16));                                \
	mlib_s32 i, ia;                                                    \
	__m128i hmask0, lmask0, hmask1, lmask1;                            \
	mlib_s32 win = (2 * level - window + 1) >> 1;                      \
	mlib_s32 ileft = ((2 * level - window) >> 1) + 1;                  \
	mlib_s32 iright = ((2 * level + window) >> 1) - 1;                 \
	mlib_u8 *edst = dst + len - 1;                                     \
	__m128i *dsp, *ddp;                                                \
	__m128i dx_0, dx_1, d0_0, d0_1, d1_0, d1_1, d2_0, d2_1, rd;        \
	__m128i dwin = _mm_set1_epi32((win << 16) | (win & 0xffff));       \
	__m128i GMIN =                                                     \
	_mm_set1_epi32((gmin << 16) | gmin);                               \
	__m128i GMAX =                                                     \
	_mm_set1_epi32((gmax << 16) | gmax);                               \
	__m128i DMIN =                                                     \
	_mm_set1_epi32((ileft << 16) | (ileft & 0xffff));                  \
	__m128i DMAX =                                                     \
	_mm_set1_epi32((iright << 16) | (iright & 0xffff));                \
	__m128i dgmin

/* *********************************************************** */

#define	MLIB_CALC1                                                     \
	lmask0 = _mm_cmplt_epi16(dx_0, DMIN);                          \
	lmask1 = _mm_cmplt_epi16(dx_1, DMIN);                          \
	hmask0 = _mm_cmpgt_epi16(dx_0, DMAX);                          \
	hmask1 = _mm_cmpgt_epi16(dx_1, DMAX);                          \
	d0_0 = _mm_sub_epi16(dx_0, dwin);                              \
	d0_1 = _mm_sub_epi16(dx_1, dwin);                              \
	d1_0 = _mm_mulhi_epi16(A, d0_0);                               \
	d1_1 = _mm_mulhi_epi16(A, d0_1);                               \
	d1_0 = _mm_slli_epi16(d1_0, 1);                                \
	d1_1 = _mm_slli_epi16(d1_1, 1);                                \
	d2_0 = _mm_add_epi16(d1_0, dgmin);                             \
	d2_1 = _mm_add_epi16(d1_1, dgmin);                             \
	d2_0 = _mm_srai_epi16(d2_0, scale);                            \
	d2_1 = _mm_srai_epi16(d2_1, scale);                            \
	d2_0 = _mm_or_si128(_mm_and_si128(lmask0, GMIN),               \
			_mm_andnot_si128(lmask0, d2_0));               \
	d2_1 = _mm_or_si128(_mm_and_si128(lmask1, GMIN),               \
			_mm_andnot_si128(lmask1, d2_1));               \
	d2_0 = _mm_or_si128(_mm_and_si128(hmask0, GMAX),               \
			_mm_andnot_si128(hmask0, d2_0));               \
	d2_1 = _mm_or_si128(_mm_and_si128(hmask1, GMAX),               \
			_mm_andnot_si128(hmask1, d2_1));               \
	rd = _mm_packus_epi16(d2_0, d2_1);

/* *********************************************************** */

#define	UNMERGE_FMUL8X16                                               \
	d0_0 = _mm_slli_epi16(d0_0, 8);                                \
	d0_1 = _mm_slli_epi16(d0_1, 8);                                \
	d0_0 = _mm_srli_epi16(d0_0, 1);                                \
	d0_1 = _mm_srli_epi16(d0_1, 1);                                \
	d1_0 = _mm_mulhi_epi16(d0_0, A);                               \
	d1_1 = _mm_mulhi_epi16(d0_1, A);                               \
	d1_0 = _mm_slli_epi16(d1_0, 1);                                \
	d1_1 = _mm_slli_epi16(d1_1, 1);

/* *********************************************************** */

#define	MLIB_CALC2                                                     \
	lmask0 = _mm_cmplt_epi16(dx_0, DMIN);                          \
	lmask1 = _mm_cmplt_epi16(dx_1, DMIN);                          \
	hmask0 = _mm_cmpgt_epi16(dx_0, DMAX);                          \
	hmask1 = _mm_cmpgt_epi16(dx_1, DMAX);                          \
	d0_0 = _mm_sub_epi16(dx_0, dwin);                              \
	d0_1 = _mm_sub_epi16(dx_1, dwin);                              \
	UNMERGE_FMUL8X16;                                              \
	d2_0 = _mm_add_epi16(d1_0, dgmin);                             \
	d2_1 = _mm_add_epi16(d1_1, dgmin);                             \
	d2_0 = _mm_srai_epi16(d2_0, scale);                            \
	d2_1 = _mm_srai_epi16(d2_1, scale);                            \
	d2_0 = _mm_or_si128(_mm_and_si128(lmask0, GMIN),               \
			_mm_andnot_si128(lmask0, d2_0));               \
	d2_1 = _mm_or_si128(_mm_and_si128(lmask1, GMIN),               \
			_mm_andnot_si128(lmask1, d2_1));               \
	d2_0 = _mm_or_si128(_mm_and_si128(hmask0, GMAX),               \
			_mm_andnot_si128(hmask0, d2_0));               \
	d2_1 = _mm_or_si128(_mm_and_si128(hmask1, GMAX),               \
			_mm_andnot_si128(hmask1, d2_1));               \
	rd = _mm_packus_epi16(d2_0, d2_1);

/* *********************************************************** */

#define	MAIN_LOOP_ALIGN(MLIB_CALC)                                     \
	for (i = 0; i < (len >> 4); i++) {                             \
	    dx_0 = _mm_load_si128(dsp);                               \
	    dx_1 = _mm_load_si128(dsp + 1);                           \
	    dsp += 2;                                                  \
	    MLIB_CALC;                                                 \
	    _mm_store_si128(ddp, rd);                                 \
	    ddp++;                                                     \
	}

#define	MAIN_LOOP_NOT_ALIGN(MLIB_CALC)                                 \
	for (i = 0; i < (len >> 4); i++) {                             \
	    dx_0 = _mm_load_si128(dsp);                               \
	    dx_1 = _mm_load_si128(dsp + 1);                           \
	    dsp += 2;                                                  \
	    MLIB_CALC;                                                 \
	    _mm_storeu_si128(ddp, rd);                                 \
	    ddp++;                                                     \
	}

/* *********************************************************** */

static void
mlib_VolumeWindowLevel1(
	mlib_u8 *dst,
	const mlib_s16 *src,
	mlib_s32 window,
	mlib_s32 level,
	mlib_s32 gmax,
	mlib_s32 gmin,
	mlib_s32 len)
{
	INIT_VARS;
	__m128i A;
	mlib_s32 count;

	while (2 * a < 1 && scale < 7) {
		a *= 2;
		scale++;
	}

	ia = a * 256.0 + 0.5;

	if (ia > MLIB_U8_MAX)
		ia = MLIB_U8_MAX;

	A = _mm_set1_epi16(((mlib_u32)(ia << 8)) >> 1);

	dgmin = _mm_set1_epi32(((gmin << 16) | gmin) << scale);

	if (window >= (1 << 15)) {
		dwin = _mm_setzero_si128();
		ia = ((gmax + gmin) << scale) * 0.5 - level * a;
		dgmin = _mm_set1_epi32((ia << 16) | (ia & 0xFFFF));
	}

	count = (16 - (((mlib_addr)src) & 15)) >> 1;
	if (count > len) count = len;
	if (count == 8) count = 0;

	for (i = 0; i < count; i++) {
		s = src[i];
		rsl = ((ai * (s - ileft)) >> 16) + gmin;
		hmask = (iright - s) >> 16;
		lmask = (s - ileft) >> 16;
		dst[i] = (rsl & ~(lmask | hmask)) |
			(gmin & lmask) | (gmax & hmask);
	}

	src += count;
	dst += count;
	dsp = (__m128i *)(src);
	ddp = (__m128i *)(dst);
	len -= count;

	if (!((mlib_addr)ddp & 15)) {
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
	MAIN_LOOP_ALIGN(MLIB_CALC1);
	} else {
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
	MAIN_LOOP_NOT_ALIGN(MLIB_CALC1);
	}

	i <<= 4;

	for (; i < len; i++) {
		s = src[i];
		rsl = ((ai * (s - ileft)) >> 16) + gmin;
		hmask = (iright - s) >> 16;
		lmask = (s - ileft) >> 16;
		dst[i] = (rsl & ~(lmask | hmask)) |
			(gmin & lmask) | (gmax & hmask);
	}
}

/* *********************************************************** */

static void
mlib_VolumeWindowLevel2(
	mlib_u8 *dst,
	const mlib_s16 *src,
	mlib_s32 window,
	mlib_s32 level,
	mlib_s32 gmax,
	mlib_s32 gmin,
	mlib_s32 len)
{
	INIT_VARS;
	__m128i A;
	mlib_s32 count;

	while (2 * a < (1 << 7) && scale < 7) {
		a *= 2;
		scale++;
	}

	ia = a * 256.0 + 0.5;

	if (ia > MLIB_S16_MAX)
		ia = MLIB_S16_MAX;

	A = _mm_set1_epi16(ia & 0xFFFF);

	dgmin = _mm_set1_epi32(((gmin << 16) | gmin) << scale);

	count = (16 - (((mlib_addr)src) & 15)) >> 1;
	if (count > len) count = len;
	if (count == 8) count = 0;

	for (i = 0; i < count; i++) {
		s = src[i];
		rsl = ((ai * (s - ileft)) >> 16) + gmin;
		hmask = (iright - s) >> 16;
		lmask = (s - ileft) >> 16;
		dst[i] = (rsl & ~(lmask | hmask)) |
			(gmin & lmask) | (gmax & hmask);
	}

	src += count;
	dst += count;
	dsp = (__m128i *)(src);
	ddp = (__m128i *)(dst);
	len -= count;

	if (!((mlib_addr)ddp & 15)) {
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
	MAIN_LOOP_ALIGN(MLIB_CALC2);
	} else {
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
	MAIN_LOOP_NOT_ALIGN(MLIB_CALC2);
	}

	i <<= 4;

	for (; i < len; i++) {
		s = src[i];
		rsl = ((ai * (s - ileft)) >> 16) + gmin;
		hmask = (iright - s) >> 16;
		lmask = (s - ileft) >> 16;
		dst[i] = (rsl & ~(lmask | hmask)) |
			(gmin & lmask) | (gmax & hmask);
	}

}

/* *********************************************************** */

mlib_status
__mlib_VolumeWindowLevel(
	mlib_u8 *dst,
	const mlib_s16 *src,
	mlib_s32 window,
	mlib_s32 level,
	mlib_s32 gmax,
	mlib_s32 gmin,
	mlib_s32 len)
{
	mlib_s32 left = ((2 * level - window) >> 1) + 1;
	mlib_s32 right = ((2 * level + window) >> 1) - 1;

	if (dst == NULL || src == NULL)
		return (MLIB_FAILURE);

	if (window <= 0 || gmin > gmax || gmax > MLIB_U8_MAX ||
		gmin < MLIB_U8_MIN)
		return (MLIB_FAILURE);

	if (left < MLIB_S16_MIN || right > MLIB_S16_MAX || left > right)
		return (MLIB_FAILURE);

	if (len <= 0)
		return (MLIB_FAILURE);

	if (window < (gmax - gmin)) {
		mlib_VolumeWindowLevel2(dst, src, window, level, gmax, gmin,
			len);
	} else {
		mlib_VolumeWindowLevel1(dst, src, window, level, gmax, gmin,
			len);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
