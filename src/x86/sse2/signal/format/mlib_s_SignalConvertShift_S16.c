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

#pragma ident	"@(#)mlib_s_SignalConvertShift_S16.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalConvertShift_[S16_S32|S16S_S32S]_Sat,
 *      mlib_SignalConvertShift_[S16_S8|S16S_S8S]_Sat,
 *      mlib_SignalConvertShift_[S16_U8|S16S_U8S]_Sat,
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalConvertShift_S16S_S32S_Sat(mlib_s16       *dst,
 *                                                        const mlib_s32 *src,
 *                                                        mlib_s32       shift,
 *                                                        mlib_s32       n);
 *      mlib_status mlib_SignalConvertShift_S16S_S8S_Sat(mlib_s16      *dst,
 *                                                       const mlib_s8 *src,
 *                                                       mlib_s32      shift,
 *                                                       mlib_s32      n);
 *      mlib_status mlib_SignalConvertShift_S16S_U8S_Sat(mlib_s16      *dst,
 *                                                       const mlib_u8 *src,
 *                                                       mlib_s32      shift,
 *                                                       mlib_s32      n);
 *      mlib_status mlib_SignalConvertShift_S16_S32_Sat(mlib_s16       *dst,
 *                                                      const mlib_s32 *src,
 *                                                      mlib_s32       shift,
 *                                                      mlib_s32       n);
 *      mlib_status mlib_SignalConvertShift_S16_S8_Sat(mlib_s16      *dst,
 *                                                     const mlib_s8 *src,
 *                                                     mlib_s32      shift,
 *                                                     mlib_s32      n);
 *      mlib_status mlib_SignalConvertShift_S16_U8_Sat(mlib_s16      *dst,
 *                                                     const mlib_u8 *src,
 *                                                     mlib_s32      shift,
 *                                                     mlib_s32      n);
 *
 * ARGUMENTS
 *      dst   Output signal array
 *      src   Input signal array
 *      shift Left shifting factor, if negative - shifts right
 *      n     Number of samples in the input signal arrays.
 *
 * DESCRIPTION
 *    - left shift of Q-format signal arrays
 *      dst = src * 2**shift;
 *      dst = saturate( dst );
 */

#include <mlib_signal.h>
#include <mlib_algebra.h>
#include <mlib_Utils.h>
#ifdef	__SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalConvertShift_S16_S32_Sat = \
	__mlib_SignalConvertShift_S16_S32_Sat
#pragma weak mlib_SignalConvertShift_S16_S8_Sat = \
	__mlib_SignalConvertShift_S16_S8_Sat
#pragma weak mlib_SignalConvertShift_S16_U8_Sat = \
	__mlib_SignalConvertShift_S16_U8_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalConvertShift_S16_S32_Sat)
    mlib_SignalConvertShift_S16_S32_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_S16_S32_Sat")));
__typeof__(__mlib_SignalConvertShift_S16_S8_Sat)
    mlib_SignalConvertShift_S16_S8_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_S16_S8_Sat")));
__typeof__(__mlib_SignalConvertShift_S16_U8_Sat)
    mlib_SignalConvertShift_S16_U8_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_S16_U8_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_S16_S8_Sat(
    mlib_s16 *dst,
    const mlib_s8 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_d64 d_val;
	__m128i *psrc;
	__m128i *pdst;
	__m128i dx, dx0, dx1, dy0, dy1, dmulter;
	__m128i fzero = _mm_setzero_si128();
	mlib_s32 ashift;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	if (shift > 7)
		shift = 7;

	if (shift < -15)
		shift = -15;
	ashift = shift + 8;

	if (n < 16 || (mlib_addr)src & 15) {
		mlib_s32 count;
		mlib_s8 *psrc_n = (mlib_s8 *)src;
		mlib_s16 *pdst_n = (mlib_s16 *)dst;

		count = (n < 16) ? n : ((16 - (mlib_addr)src) & 15);

		if (shift <= 0) {
			for (i = 0; i < count; i++) {
				pdst_n[i] =
				    ((ashift >=
				    0) ? (psrc_n[i] << ashift) : (psrc_n[i] >>
				    (-ashift)));
			}
		} else {
			d_val = ((mlib_u32)1) << (shift + 24);
			for (i = 0; i < count; i++) {
				SAT16(pdst_n[i], d_val * psrc_n[i]);
			}
		}

		n -= count;
		dst = (void *)((mlib_s16 *)dst + count);
		src = (void *)((mlib_s8 *)src + count);
	}

/* psrc is 128bit-aligned now */

	psrc = (__m128i *)src;
	pdst = (__m128i *)dst;

	if (!(15 & (mlib_addr)pdst)) {
		if (ashift <= 8) {
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			for (i = 0; i < n >> 4; i++) {
				dx = _mm_load_si128(psrc + i);
				dy0 = _mm_unpacklo_epi8(dx, fzero);
				dy1 = _mm_unpackhi_epi8(dx, fzero);
				dy0 = _mm_slli_epi16(dy0, 8);
				dy1 = _mm_slli_epi16(dy1, 8);
				dy0 = _mm_srai_epi16(dy0, 8 - ashift);
				dy1 = _mm_srai_epi16(dy1, 8 - ashift);
				_mm_store_si128(pdst + 2 * i, dy0);
				_mm_store_si128(pdst + 2 * i + 1, dy1);
			}
		} else {
			__m128i dy2, dy3;
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			for (i = 0; i < n >> 4; i++) {
				dx = _mm_load_si128(psrc + i);
				dy2 = _mm_unpacklo_epi8(dx, fzero);
				dy3 = _mm_unpackhi_epi8(dx, fzero);
				dy0 = _mm_unpacklo_epi16(dy2, fzero);
				dy1 = _mm_unpacklo_epi16(dy3, fzero);
				dy2 = _mm_unpackhi_epi16(dy2, fzero);
				dy3 = _mm_unpackhi_epi16(dy3, fzero);
				dy0 = _mm_slli_epi32(dy0, 24);
				dy1 = _mm_slli_epi32(dy1, 24);
				dy2 = _mm_slli_epi32(dy2, 24);
				dy3 = _mm_slli_epi32(dy3, 24);
				dy0 = _mm_srai_epi32(dy0, 24 - ashift);
				dy1 = _mm_srai_epi32(dy1, 24 - ashift);
				dy2 = _mm_srai_epi32(dy2, 24 - ashift);
				dy3 = _mm_srai_epi32(dy3, 24 - ashift);
				dy0 = _mm_packs_epi32(dy0, dy2);
				dy1 = _mm_packs_epi32(dy1, dy3);
				_mm_store_si128(pdst + 2 * i, dy0);
				_mm_store_si128(pdst + 2 * i + 1, dy1);
			}
		}
	} else {
		if (ashift <= 8) {
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			for (i = 0; i < n >> 4; i++) {
				dx = _mm_load_si128(psrc + i);
				dy0 = _mm_unpacklo_epi8(dx, fzero);
				dy1 = _mm_unpackhi_epi8(dx, fzero);
				dy0 = _mm_slli_epi16(dy0, 8);
				dy1 = _mm_slli_epi16(dy1, 8);
				dy0 = _mm_srai_epi16(dy0, 8 - ashift);
				dy1 = _mm_srai_epi16(dy1, 8 - ashift);
				_mm_storeu_si128(pdst + 2 * i, dy0);
				_mm_storeu_si128(pdst + 2 * i + 1, dy1);
			}
		} else {
			__m128i dy2, dy3;
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			for (i = 0; i < n >> 4; i++) {
				dx = _mm_load_si128(psrc + i);
				dy2 = _mm_unpacklo_epi8(dx, fzero);
				dy3 = _mm_unpackhi_epi8(dx, fzero);
				dy0 = _mm_unpacklo_epi16(dy2, fzero);
				dy1 = _mm_unpacklo_epi16(dy3, fzero);
				dy2 = _mm_unpackhi_epi16(dy2, fzero);
				dy3 = _mm_unpackhi_epi16(dy3, fzero);
				dy0 = _mm_slli_epi32(dy0, 24);
				dy1 = _mm_slli_epi32(dy1, 24);
				dy2 = _mm_slli_epi32(dy2, 24);
				dy3 = _mm_slli_epi32(dy3, 24);
				dy0 = _mm_srai_epi32(dy0, 24 - ashift);
				dy1 = _mm_srai_epi32(dy1, 24 - ashift);
				dy2 = _mm_srai_epi32(dy2, 24 - ashift);
				dy3 = _mm_srai_epi32(dy3, 24 - ashift);
				dy0 = _mm_packs_epi32(dy0, dy2);
				dy1 = _mm_packs_epi32(dy1, dy3);
				_mm_storeu_si128(pdst + 2 * i, dy0);
				_mm_storeu_si128(pdst + 2 * i + 1, dy1);
			}
		}
	}

	i <<= 4;

	if (i < n) {
		mlib_s8 *psrc_n = (mlib_s8 *)psrc;
		mlib_s16 *pdst_n = (mlib_s16 *)pdst;

		if (shift <= 0) {
			for (; i < n; i++) {
				pdst_n[i] =
				    (ashift >=
				    0) ? (psrc_n[i] << ashift) : (psrc_n[i] >>
				    (-ashift));
			}
		} else {
			d_val = ((mlib_u32)1) << (shift + 24);
			for (; i < n; i++) {
				SAT16(pdst_n[i], d_val * psrc_n[i]);
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_S16_U8_Sat(
    mlib_s16 *dst,
    const mlib_u8 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_d64 d_val;
	__m128i *psrc;
	__m128i *pdst;
	__m128i dx, dx0, dx1, dy0, dy1, dy2, dy3;
	__m128i fzero = _mm_setzero_si128();
	mlib_s32 ashift;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	if (shift > 8)
		shift = 8;
	ashift = shift + 7;

	if (shift <= -15) {
/* set output to zero */
		return (__mlib_VectorZero_S16(dst, n));
	}

	if (n < 16 || (mlib_addr)src & 15) {
		mlib_s32 count;
		mlib_u8 *psrc_n = (mlib_u8 *)src;
		mlib_s16 *pdst_n = (mlib_s16 *)dst;

		count = (n < 16) ? n : (16 - (mlib_addr)src) & 15;

		if (shift <= 0) {
			for (i = 0; i < count; i++) {
				pdst_n[i] =
				    ((ashift >=
				    0) ? (psrc_n[i] << ashift) : (psrc_n[i] >>
				    (-ashift)));
			}
		} else {
			d_val = ((mlib_u32)1) << (ashift + 16);
			for (i = 0; i < count; i++) {
				SAT16(pdst_n[i], d_val * psrc_n[i]);
			}
		}

		n -= count;
		dst = (void *)((mlib_s16 *)dst + count);
		src = (void *)((mlib_u8 *)src + count);
	}

/* psrc is 128bit-aligned now */

	psrc = (__m128i *)src;
	pdst = (__m128i *)dst;

	if (!(15 & (mlib_addr)pdst)) {
		if (ashift < 0) {
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			for (i = 0; i < n >> 4; i++) {
				dx = _mm_load_si128(psrc + i);
				dy0 = _mm_unpacklo_epi8(dx, fzero);
				dy1 = _mm_unpackhi_epi8(dx, fzero);
				dy0 = _mm_srai_epi16(dy0, -ashift);
				dy1 = _mm_srai_epi16(dy1, -ashift);
				_mm_store_si128(pdst + 2 * i, dy0);
				_mm_store_si128(pdst + 2 * i + 1, dy1);
			}
		} else if (ashift <= 7) {
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			for (i = 0; i < n >> 4; i++) {
				dx = _mm_load_si128(psrc + i);
				dy0 = _mm_unpacklo_epi8(dx, fzero);
				dy1 = _mm_unpackhi_epi8(dx, fzero);
				dy0 = _mm_slli_epi16(dy0, ashift);
				dy1 = _mm_slli_epi16(dy1, ashift);
				_mm_store_si128(pdst + 2 * i, dy0);
				_mm_store_si128(pdst + 2 * i + 1, dy1);
			}
		} else {
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			for (i = 0; i < n >> 4; i++) {
				dx = _mm_load_si128(psrc + i);
				dy2 = _mm_unpacklo_epi8(dx, fzero);
				dy3 = _mm_unpackhi_epi8(dx, fzero);
				dy0 = _mm_unpacklo_epi16(dy2, fzero);
				dy1 = _mm_unpacklo_epi16(dy3, fzero);
				dy2 = _mm_unpackhi_epi16(dy2, fzero);
				dy3 = _mm_unpackhi_epi16(dy3, fzero);
				dy0 = _mm_slli_epi32(dy0, ashift);
				dy1 = _mm_slli_epi32(dy1, ashift);
				dy2 = _mm_slli_epi32(dy2, ashift);
				dy3 = _mm_slli_epi32(dy3, ashift);
				dy0 = _mm_packs_epi32(dy0, dy2);
				dy1 = _mm_packs_epi32(dy1, dy3);
				_mm_store_si128(pdst + 2 * i, dy0);
				_mm_store_si128(pdst + 2 * i + 1, dy1);
			}
		}
	} else {
		if (ashift < 0) {
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			for (i = 0; i < n >> 4; i++) {
				dx = _mm_load_si128(psrc + i);
				dy0 = _mm_unpacklo_epi8(dx, fzero);
				dy1 = _mm_unpackhi_epi8(dx, fzero);
				dy0 = _mm_srai_epi16(dy0, -ashift);
				dy1 = _mm_srai_epi16(dy1, -ashift);
				_mm_storeu_si128(pdst + 2 * i, dy0);
				_mm_storeu_si128(pdst + 2 * i + 1, dy1);
			}
		} else if (ashift <= 7) {
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			for (i = 0; i < n >> 4; i++) {
				dx = _mm_load_si128(psrc + i);
				dy0 = _mm_unpacklo_epi8(dx, fzero);
				dy1 = _mm_unpackhi_epi8(dx, fzero);
				dy0 = _mm_slli_epi16(dy0, ashift);
				dy1 = _mm_slli_epi16(dy1, ashift);
				_mm_storeu_si128(pdst + 2 * i, dy0);
				_mm_storeu_si128(pdst + 2 * i + 1, dy1);
			}
		} else {
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			for (i = 0; i < n >> 4; i++) {
				dx = _mm_load_si128(psrc + i);
				dy2 = _mm_unpacklo_epi8(dx, fzero);
				dy3 = _mm_unpackhi_epi8(dx, fzero);
				dy0 = _mm_unpacklo_epi16(dy2, fzero);
				dy1 = _mm_unpacklo_epi16(dy3, fzero);
				dy2 = _mm_unpackhi_epi16(dy2, fzero);
				dy3 = _mm_unpackhi_epi16(dy3, fzero);
				dy0 = _mm_slli_epi32(dy0, ashift);
				dy1 = _mm_slli_epi32(dy1, ashift);
				dy2 = _mm_slli_epi32(dy2, ashift);
				dy3 = _mm_slli_epi32(dy3, ashift);
				dy0 = _mm_packs_epi32(dy0, dy2);
				dy1 = _mm_packs_epi32(dy1, dy3);
				_mm_storeu_si128(pdst + 2 * i, dy0);
				_mm_storeu_si128(pdst + 2 * i + 1, dy1);
			}
		}
	}

	i <<= 4;

	if (i < n) {
		mlib_u8 *psrc_n = (mlib_u8 *)psrc;
		mlib_s16 *pdst_n = (mlib_s16 *)pdst;

		if (shift <= 0) {
			for (; i < n; i++) {
				pdst_n[i] =
				    (ashift >=
				    0) ? (psrc_n[i] << ashift) : (psrc_n[i] >>
				    (-ashift));
			}
		} else {
			d_val = ((mlib_u32)1) << (ashift + 16);
			for (; i < n; i++) {
				SAT16(pdst_n[i], d_val * psrc_n[i]);
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	SAT_S16_P(x)                                                \
			mask0 = _mm_sub_epi64(ceiling, (x));        \
			mask1 = _mm_sub_epi64((x), floor);          \
			mask0 = _mm_srai_epi32(mask0, 31);          \
			mask0 = _mm_and_si128(mask0, mask);         \
			mask2 = _mm_srli_si128(mask0, 4);           \
			mask0 = _mm_or_si128(mask0, mask2);         \
			mask1 = _mm_srai_epi32(mask1, 31);          \
			mask1 = _mm_and_si128(mask1, mask);         \
			mask2 = _mm_srli_si128(mask1, 4);           \
			mask1 = _mm_or_si128(mask1, mask2);         \
			mask2 = _mm_and_si128(mask0, ceiling);      \
			mask3 = _mm_andnot_si128(mask0, (x));       \
			(x) = _mm_or_si128(mask2, mask3);           \
			mask2 = _mm_and_si128(mask1, floor);        \
			mask3 = _mm_andnot_si128(mask1, (x));       \
			(x) = _mm_or_si128(mask2, mask3);

mlib_status
__mlib_SignalConvertShift_S16_S32_Sat(
    mlib_s16 *dst,
    const mlib_s32 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_d64 dmulter;
	__m128i *psrc;
	__m128i *pdst;
	__m128i dx, dy0, dy1, dy2, dy3;
	mlib_s32 ashift;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	if (shift > 31)
		shift = 31;

	if (shift < -15)
		shift = -15;
	ashift = shift - 16;

	if (n < 8 || (mlib_addr)src & 15) {
		mlib_s32 count;
		mlib_d64 d_val;
		mlib_s32 *psrc_n = (mlib_s32 *)src;
		mlib_s16 *pdst_n = (mlib_s16 *)dst;

		count = (n < 8) ? n : ((16 - (mlib_addr)src) & 15) >> 2;

		if (shift <= 0) {
			for (i = 0; i < count; i++) {
				pdst_n[i] = psrc_n[i] >> (-ashift);
			}
		} else {
			d_val = ((mlib_u32)1) << shift;
			for (i = 0; i < count; i++) {
				SAT16(pdst_n[i], d_val * psrc_n[i]);
			}
		}

		if (n < 8)
			return (MLIB_SUCCESS);
		n -= count;
		dst = (void *)((mlib_s16 *)dst + count);
		src = (void *)((mlib_s32 *)src + count);
	}

/* psrc is 128bit-aligned now */

	psrc = (__m128i *)src;
	pdst = (__m128i *)dst;

	if (!(15 & (mlib_addr)pdst)) {

		if (ashift <= 0) {
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			for (i = 0; i < n >> 3; i++) {
				dy0 = _mm_load_si128(psrc + 2 * i);
				dy1 = _mm_load_si128(psrc + 2 * i + 1);
				dy0 = _mm_srai_epi32(dy0, -ashift);
				dy1 = _mm_srai_epi32(dy1, -ashift);
				dx = _mm_packs_epi32(dy0, dy1);
				_mm_store_si128(pdst + i, dx);
			}
		} else {
			__m128i dy4, dy5, dy6, dy7;
			__m128i mask0, mask1, mask2, mask3;
			__m128i ceiling = _mm_set_epi32(0, 0x7fff, 0, 0x7fff);
			__m128i floor = _mm_set_epi32(0xffffffff, 0xffff8000,
					0xffffffff, 0xffff8000);
			__m128i mask = _mm_cmpeq_epi32(mask, mask);
			mask = _mm_slli_epi64(mask, 32);
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			for (i = 0; i < n >> 3; i++) {
				dy0 = _mm_load_si128(psrc + 2 * i);
				dy1 = _mm_load_si128(psrc + 2 * i + 1);
				mask0 = _mm_srai_epi32(dy0, 31);
				mask1 = _mm_srai_epi32(dy1, 31);
				dy4 = _mm_unpackhi_epi32(dy0, mask0);
				dy0 = _mm_unpacklo_epi32(dy0, mask0);
				dy5 = _mm_unpackhi_epi32(dy1, mask1);
				dy1 = _mm_unpacklo_epi32(dy1, mask1);
				dy4 = _mm_slli_epi64(dy4, ashift);
				dy0 = _mm_slli_epi64(dy0, ashift);
				dy5 = _mm_slli_epi64(dy5, ashift);
				dy1 = _mm_slli_epi64(dy1, ashift);

				SAT_S16_P(dy4);
				SAT_S16_P(dy0);
				SAT_S16_P(dy5);
				SAT_S16_P(dy1);

				dy2 = _mm_unpacklo_epi16(dy0, dy1);
				dy1 = _mm_unpackhi_epi16(dy0, dy1);
				dy3 = _mm_unpacklo_epi16(dy4, dy5);
				dy5 = _mm_unpackhi_epi16(dy4, dy5);

				dy2 = _mm_unpacklo_epi16(dy2, dy3);
				dy1 = _mm_unpacklo_epi16(dy1, dy5);

				dy0 = _mm_unpacklo_epi16(dy2, dy1);
				_mm_store_si128(pdst + i, dy0);
			}
		}
	} else {

		if (ashift <= 0) {
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			for (i = 0; i < n >> 3; i++) {
				dy0 = _mm_load_si128(psrc + 2 * i);
				dy1 = _mm_load_si128(psrc + 2 * i + 1);
				dy0 = _mm_srai_epi32(dy0, -ashift);
				dy1 = _mm_srai_epi32(dy1, -ashift);
				dx = _mm_packs_epi32(dy0, dy1);
				_mm_storeu_si128(pdst + i, dx);
			}
		} else {
			__m128i dy4, dy5, dy6, dy7;
			__m128i mask0, mask1, mask2, mask3;
			__m128i ceiling = _mm_set_epi32(0, 0x7fff, 0, 0x7fff);
			__m128i floor = _mm_set_epi32(0xffffffff, 0xffff8000,
					0xffffffff, 0xffff8000);
			__m128i mask = _mm_cmpeq_epi32(mask, mask);
			mask = _mm_slli_epi64(mask, 32);
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			for (i = 0; i < n >> 3; i++) {
				dy0 = _mm_load_si128(psrc + 2 * i);
				dy1 = _mm_load_si128(psrc + 2 * i + 1);
				mask0 = _mm_srai_epi32(dy0, 31);
				mask1 = _mm_srai_epi32(dy1, 31);
				dy4 = _mm_unpackhi_epi32(dy0, mask0);
				dy0 = _mm_unpacklo_epi32(dy0, mask0);
				dy5 = _mm_unpackhi_epi32(dy1, mask1);
				dy1 = _mm_unpacklo_epi32(dy1, mask1);
				dy4 = _mm_slli_epi64(dy4, ashift);
				dy0 = _mm_slli_epi64(dy0, ashift);
				dy5 = _mm_slli_epi64(dy5, ashift);
				dy1 = _mm_slli_epi64(dy1, ashift);

				SAT_S16_P(dy4);
				SAT_S16_P(dy0);
				SAT_S16_P(dy5);
				SAT_S16_P(dy1);

				dy2 = _mm_unpacklo_epi16(dy0, dy1);
				dy1 = _mm_unpackhi_epi16(dy0, dy1);
				dy3 = _mm_unpacklo_epi16(dy4, dy5);
				dy5 = _mm_unpackhi_epi16(dy4, dy5);

				dy2 = _mm_unpacklo_epi16(dy2, dy3);
				dy1 = _mm_unpacklo_epi16(dy1, dy5);

				dy0 = _mm_unpacklo_epi16(dy2, dy1);
				_mm_storeu_si128(pdst + i, dy0);
			}
		}
	}

	i <<= 3;

	if (i < n) {
		mlib_d64 d_val;
		mlib_s32 *psrc_n = (mlib_s32 *)psrc;
		mlib_s16 *pdst_n = (mlib_s16 *)pdst;

		if (shift <= 0) {
			for (; i < n; i++) {
				pdst_n[i] = psrc_n[i] >> (-ashift);
			}
		} else {
			d_val = ((mlib_u32)1) << shift;
			for (; i < n; i++) {
				SAT16(pdst_n[i], d_val * psrc_n[i]);
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
