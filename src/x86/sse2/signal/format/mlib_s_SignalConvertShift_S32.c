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

#pragma ident	"@(#)mlib_s_SignalConvertShift_S32.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalConvertShift_[S32_S16|S32S_S16S]_Sat,
 *      mlib_SignalConvertShift_[S32_S8|S32S_S8S]_Sat,
 *      mlib_SignalConvertShift_[S32_U8|S32S_U8S]_Sat,
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalConvertShift_S32S_S16S_Sat(mlib_s32       *dst,
 *                                                        const mlib_s16 *src,
 *                                                        mlib_s32       shift,
 *                                                        mlib_s32       n);
 *      mlib_status mlib_SignalConvertShift_S32S_S8S_Sat(mlib_s32      *dst,
 *                                                       const mlib_s8 *src,
 *                                                       mlib_s32      shift,
 *                                                       mlib_s32      n);
 *      mlib_status mlib_SignalConvertShift_S32S_U8S_Sat(mlib_s32      *dst,
 *                                                       const mlib_u8 *src,
 *                                                       mlib_s32      shift,
 *                                                       mlib_s32      n);
 *      mlib_status mlib_SignalConvertShift_S32_S16_Sat(mlib_s32       *dst,
 *                                                      const mlib_s16 *src,
 *                                                      mlib_s32       shift,
 *                                                      mlib_s32       n);
 *      mlib_status mlib_SignalConvertShift_S32_S8_Sat(mlib_s32      *dst,
 *                                                     const mlib_s8 *src,
 *                                                     mlib_s32      shift,
 *                                                     mlib_s32      n);
 *      mlib_status mlib_SignalConvertShift_S32_U8_Sat(mlib_s32      *dst,
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

#pragma weak mlib_SignalConvertShift_S32_S16_Sat = \
	__mlib_SignalConvertShift_S32_S16_Sat
#pragma weak mlib_SignalConvertShift_S32_S8_Sat = \
	__mlib_SignalConvertShift_S32_S8_Sat
#pragma weak mlib_SignalConvertShift_S32_U8_Sat = \
	__mlib_SignalConvertShift_S32_U8_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalConvertShift_S32_S16_Sat)
    mlib_SignalConvertShift_S32_S16_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_S32_S16_Sat")));
__typeof__(__mlib_SignalConvertShift_S32_S8_Sat)
    mlib_SignalConvertShift_S32_S8_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_S32_S8_Sat")));
__typeof__(__mlib_SignalConvertShift_S32_U8_Sat)
    mlib_SignalConvertShift_S32_U8_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_S32_U8_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	SAT_S32_P(x)                                        \
		mask0 = _mm_sub_epi64(ceiling, (x));        \
		mask1 = _mm_sub_epi64((x), floor);          \
		mask0 = _mm_srli_si128(mask0, 4);           \
		mask1 = _mm_srli_si128(mask1, 4);           \
		mask0 = _mm_srai_epi32(mask0, 31);          \
		mask1 = _mm_srai_epi32(mask1, 31);          \
		mask2 = _mm_and_si128(mask0, ceiling);      \
		mask3 = _mm_andnot_si128(mask0, (x));       \
		(x) = _mm_or_si128(mask2, mask3);           \
		mask2 = _mm_and_si128(mask1, floor);        \
		mask3 = _mm_andnot_si128(mask1, (x));       \
		(x) = _mm_or_si128(mask2, mask3);

mlib_status
__mlib_SignalConvertShift_S32_S8_Sat(
    mlib_s32 *dst,
    const mlib_s8 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_d64 d_val;
	__m128i *pdst, *psrc;
	__m128i dy0, dy1, dy2, dy3, dx0, dx1, dx2, dx3;
	mlib_s32 ashift;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	if (shift > 7)
		shift = 7;

	if (shift < -31)
		shift = -31;
	ashift = shift + 24;

	if (n < 16 || (mlib_addr)dst & 15) {
		mlib_s32 count =
			((n < 16) ? n : (15 & (16 - (mlib_addr)dst)) >> 2);

		if (ashift < 0)
			for (i = 0; i < count; ++i)
				((mlib_s32 *)dst)[i] =
				    ((mlib_s8 *)src)[i] >> (-ashift);
		else if (shift <= 0)
			for (i = 0; i < count; ++i)
				((mlib_s32 *)dst)[i] =
				    ((mlib_s8 *)src)[i] << (ashift);
		else {
			mlib_d64 d_valc = ((mlib_u32)1) << ashift;

			for (i = 0; i < count; ++i) {
				FLOAT2INT_CLAMP(((mlib_s32 *)dst)[i],
				    d_valc * ((mlib_s8 *)src)[i]);
			}
		}

		n -= count;
		src = (void *)(((mlib_s8 *)src) + count);
		dst = (void *)(((mlib_s32 *)dst) + count);
	}

/* pdst is 128bit-aligned now */
	psrc = (__m128i *)src;
	pdst = (__m128i *)dst;

	if (!(15 & (mlib_addr)psrc)) {
		if (ashift <= 24) {
#ifdef	__SUNPRO_C
#pragma	pipeloop(0)
#endif
			for (i = 0; i < n >> 4; i++) {
				dx3 = _mm_load_si128(psrc + i);
				dx1 = _mm_unpacklo_epi8(dx3, dx3);
				dx3 = _mm_unpackhi_epi8(dx3, dx3);
				dx0 = _mm_unpacklo_epi16(dx1, dx1);
				dx1 = _mm_unpackhi_epi16(dx1, dx1);
				dx2 = _mm_unpacklo_epi16(dx3, dx3);
				dx3 = _mm_unpackhi_epi16(dx3, dx3);
				dx0 = _mm_slli_epi32(dx0, 24);
				dx1 = _mm_slli_epi32(dx1, 24);
				dx2 = _mm_slli_epi32(dx2, 24);
				dx3 = _mm_slli_epi32(dx3, 24);
				dx0 = _mm_srai_epi32(dx0, 24 - ashift);
				dx1 = _mm_srai_epi32(dx1, 24 - ashift);
				dx2 = _mm_srai_epi32(dx2, 24 - ashift);
				dx3 = _mm_srai_epi32(dx3, 24 - ashift);
				_mm_store_si128(pdst + 4 * i, dx0);
				_mm_store_si128(pdst + 4 * i + 1, dx1);
				_mm_store_si128(pdst + 4 * i + 2, dx2);
				_mm_store_si128(pdst + 4 * i + 3, dx3);
			}
		} else {
			__m128i mask0, mask1, mask2, mask3;
			__m128i ceiling = _mm_set_epi32(0, 0x7fffffff,
					0, 0x7fffffff);
			__m128i floor = _mm_cmpeq_epi32(floor, floor);
			__m128i mask = _mm_slli_epi64(floor, 32);
			floor = _mm_slli_epi64(floor, 31);
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			for (i = 0; i < n >> 4; i++) {
				dx3 = _mm_load_si128(psrc + i);
				dx1 = _mm_unpacklo_epi8(dx3, dx3);
				dx3 = _mm_unpackhi_epi8(dx3, dx3);
				dx0 = _mm_unpacklo_epi16(dx1, dx1);
				dx1 = _mm_unpackhi_epi16(dx1, dx1);
				dx2 = _mm_unpacklo_epi16(dx3, dx3);
				dx3 = _mm_unpackhi_epi16(dx3, dx3);
				dx0 = _mm_slli_epi32(dx0, 24);
				dx1 = _mm_slli_epi32(dx1, 24);
				dx2 = _mm_slli_epi32(dx2, 24);
				dx3 = _mm_slli_epi32(dx3, 24);
				mask0 = _mm_srai_epi32(dx0, 31);
				mask1 = _mm_srai_epi32(dx1, 31);
				mask2 = _mm_srai_epi32(dx2, 31);
				mask3 = _mm_srai_epi32(dx3, 31);
				dy0 = _mm_unpackhi_epi32(dx0, mask0);
				dx0 = _mm_unpacklo_epi32(dx0, mask0);
				dy1 = _mm_unpackhi_epi32(dx1, mask1);
				dx1 = _mm_unpacklo_epi32(dx1, mask1);
				dy2 = _mm_unpackhi_epi32(dx2, mask2);
				dx2 = _mm_unpacklo_epi32(dx2, mask2);
				dy3 = _mm_unpackhi_epi32(dx3, mask3);
				dx3 = _mm_unpacklo_epi32(dx3, mask3);
				dy0 = _mm_slli_epi64(dy0, ashift - 24);
				dx0 = _mm_slli_epi64(dx0, ashift - 24);
				dy1 = _mm_slli_epi64(dy1, ashift - 24);
				dx1 = _mm_slli_epi64(dx1, ashift - 24);
				dy2 = _mm_slli_epi64(dy2, ashift - 24);
				dx2 = _mm_slli_epi64(dx2, ashift - 24);
				dy3 = _mm_slli_epi64(dy3, ashift - 24);
				dx3 = _mm_slli_epi64(dx3, ashift - 24);

				SAT_S32_P(dy0);
				SAT_S32_P(dx0);
				SAT_S32_P(dy1);
				SAT_S32_P(dx1);
				SAT_S32_P(dy2);
				SAT_S32_P(dx2);
				SAT_S32_P(dy3);
				SAT_S32_P(dx3);

				mask0 = _mm_unpacklo_epi32(dx0, dy0);
				dx0 = _mm_unpackhi_epi32(dx0, dy0);
				dy0 = _mm_unpacklo_epi32(mask0, dx0);
				mask1 = _mm_unpacklo_epi32(dx1, dy1);
				dx1 = _mm_unpackhi_epi32(dx1, dy1);
				dy1 = _mm_unpacklo_epi32(mask1, dx1);
				mask2 = _mm_unpacklo_epi32(dx2, dy2);
				dx2 = _mm_unpackhi_epi32(dx2, dy2);
				dy2 = _mm_unpacklo_epi32(mask2, dx2);
				mask3 = _mm_unpacklo_epi32(dx3, dy3);
				dx3 = _mm_unpackhi_epi32(dx3, dy3);
				dy3 = _mm_unpacklo_epi32(mask3, dx3);

				_mm_store_si128(pdst + 4 * i, dy0);
				_mm_store_si128(pdst + 4 * i + 1, dy1);
				_mm_store_si128(pdst + 4 * i + 2, dy2);
				_mm_store_si128(pdst + 4 * i + 3, dy3);
			}
		}
	} else {
		if (ashift <= 24) {
#ifdef	__SUNPRO_C
#pragma	pipeloop(0)
#endif
			for (i = 0; i < n >> 4; i++) {
				dx3 = _mm_loadu_si128(psrc + i);
				dx1 = _mm_unpacklo_epi8(dx3, dx3);
				dx3 = _mm_unpackhi_epi8(dx3, dx3);
				dx0 = _mm_unpacklo_epi16(dx1, dx1);
				dx1 = _mm_unpackhi_epi16(dx1, dx1);
				dx2 = _mm_unpacklo_epi16(dx3, dx3);
				dx3 = _mm_unpackhi_epi16(dx3, dx3);
				dx0 = _mm_slli_epi32(dx0, 24);
				dx1 = _mm_slli_epi32(dx1, 24);
				dx2 = _mm_slli_epi32(dx2, 24);
				dx3 = _mm_slli_epi32(dx3, 24);
				dx0 = _mm_srai_epi32(dx0, 24 - ashift);
				dx1 = _mm_srai_epi32(dx1, 24 - ashift);
				dx2 = _mm_srai_epi32(dx2, 24 - ashift);
				dx3 = _mm_srai_epi32(dx3, 24 - ashift);
				_mm_store_si128(pdst + 4 * i, dx0);
				_mm_store_si128(pdst + 4 * i + 1, dx1);
				_mm_store_si128(pdst + 4 * i + 2, dx2);
				_mm_store_si128(pdst + 4 * i + 3, dx3);
			}
		} else {
			__m128i mask0, mask1, mask2, mask3;
			__m128i ceiling = _mm_set_epi32(0, 0x7fffffff,
					0, 0x7fffffff);
			__m128i floor = _mm_cmpeq_epi32(floor, floor);
			__m128i mask = _mm_slli_epi64(floor, 32);
			floor = _mm_slli_epi64(floor, 31);
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			for (i = 0; i < n >> 4; i++) {
				dx3 = _mm_loadu_si128(psrc + i);
				dx1 = _mm_unpacklo_epi8(dx3, dx3);
				dx3 = _mm_unpackhi_epi8(dx3, dx3);
				dx0 = _mm_unpacklo_epi16(dx1, dx1);
				dx1 = _mm_unpackhi_epi16(dx1, dx1);
				dx2 = _mm_unpacklo_epi16(dx3, dx3);
				dx3 = _mm_unpackhi_epi16(dx3, dx3);
				dx0 = _mm_slli_epi32(dx0, 24);
				dx1 = _mm_slli_epi32(dx1, 24);
				dx2 = _mm_slli_epi32(dx2, 24);
				dx3 = _mm_slli_epi32(dx3, 24);
				mask0 = _mm_srai_epi32(dx0, 31);
				mask1 = _mm_srai_epi32(dx1, 31);
				mask2 = _mm_srai_epi32(dx2, 31);
				mask3 = _mm_srai_epi32(dx3, 31);
				dy0 = _mm_unpackhi_epi32(dx0, mask0);
				dx0 = _mm_unpacklo_epi32(dx0, mask0);
				dy1 = _mm_unpackhi_epi32(dx1, mask1);
				dx1 = _mm_unpacklo_epi32(dx1, mask1);
				dy2 = _mm_unpackhi_epi32(dx2, mask2);
				dx2 = _mm_unpacklo_epi32(dx2, mask2);
				dy3 = _mm_unpackhi_epi32(dx3, mask3);
				dx3 = _mm_unpacklo_epi32(dx3, mask3);
				dy0 = _mm_slli_epi64(dy0, ashift - 24);
				dx0 = _mm_slli_epi64(dx0, ashift - 24);
				dy1 = _mm_slli_epi64(dy1, ashift - 24);
				dx1 = _mm_slli_epi64(dx1, ashift - 24);
				dy2 = _mm_slli_epi64(dy2, ashift - 24);
				dx2 = _mm_slli_epi64(dx2, ashift - 24);
				dy3 = _mm_slli_epi64(dy3, ashift - 24);
				dx3 = _mm_slli_epi64(dx3, ashift - 24);

				SAT_S32_P(dy0);
				SAT_S32_P(dx0);
				SAT_S32_P(dy1);
				SAT_S32_P(dx1);
				SAT_S32_P(dy2);
				SAT_S32_P(dx2);
				SAT_S32_P(dy3);
				SAT_S32_P(dx3);

				mask0 = _mm_unpacklo_epi32(dx0, dy0);
				dx0 = _mm_unpackhi_epi32(dx0, dy0);
				dy0 = _mm_unpacklo_epi32(mask0, dx0);
				mask1 = _mm_unpacklo_epi32(dx1, dy1);
				dx1 = _mm_unpackhi_epi32(dx1, dy1);
				dy1 = _mm_unpacklo_epi32(mask1, dx1);
				mask2 = _mm_unpacklo_epi32(dx2, dy2);
				dx2 = _mm_unpackhi_epi32(dx2, dy2);
				dy2 = _mm_unpacklo_epi32(mask2, dx2);
				mask3 = _mm_unpacklo_epi32(dx3, dy3);
				dx3 = _mm_unpackhi_epi32(dx3, dy3);
				dy3 = _mm_unpacklo_epi32(mask3, dx3);

				_mm_store_si128(pdst + 4 * i, dy0);
				_mm_store_si128(pdst + 4 * i + 1, dy1);
				_mm_store_si128(pdst + 4 * i + 2, dy2);
				_mm_store_si128(pdst + 4 * i + 3, dy3);
			}
		}
	}

	i <<= 4;

	if (i < n) {
		mlib_s8 *psrc_n = (mlib_s8 *)psrc;
		mlib_s32 *pdst_n = (mlib_s32 *)pdst;

		if (ashift < 0) {
			for (; i < n; i++) {
				pdst_n[i] = psrc_n[i] >> (-ashift);
			}
		} else if (shift <= 0) {
			for (; i < n; i++) {
				pdst_n[i] = psrc_n[i] << ashift;
			}
		} else {
			mlib_d64 d_valc = ((mlib_u32)1) << ashift;

			for (; i < n; i++) {
				FLOAT2INT_CLAMP(pdst_n[i], d_valc * psrc_n[i]);
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_S32_U8_Sat(
    mlib_s32 *dst,
    const mlib_u8 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	__m128i *pdst, *psrc;
	__m128i dx0, dx1, dx2, dx3, dy0, dy1, dy2, dy3;
	__m128i fzero = _mm_setzero_si128();
	mlib_d64 d_val;
	mlib_s32 i;
	mlib_s32 ashift;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	if (shift > 8)
		shift = 8;
	ashift = shift + 23;

/* set output to zero */
	if (shift <= -31) {
		return (__mlib_VectorZero_S32(dst, n));
	}

	if (n < 16 || (mlib_addr)dst & 15) {
		mlib_s32 count =
			((n < 16) ? n : (15 & (16 - (mlib_addr)dst)) >> 2);

		if (ashift < 0)
			for (i = 0; i < count; ++i)
				((mlib_s32 *)dst)[i] =
				    ((mlib_u8 *)src)[i] >> (-ashift);
		else if (shift <= 0)
			for (i = 0; i < count; ++i)
				((mlib_s32 *)dst)[i] =
				    ((mlib_u8 *)src)[i] << (ashift);
		else {
			mlib_d64 d_valc = ((mlib_u32)1) << ashift;

			for (i = 0; i < count; ++i) {
				FLOAT2INT_CLAMP(((mlib_s32 *)dst)[i],
				    d_valc * ((mlib_u8 *)src)[i]);
			}
		}

		n -= count;
		src = (void *)(((mlib_u8 *)src) + count);
		dst = (void *)(((mlib_s32 *)dst) + count);
	}

/* pdst is 128bit-aligned now */
	psrc = (__m128i *)src;
	pdst = (__m128i *)dst;

	if (!(15 & (mlib_addr)psrc)) {
		if (ashift < 0) {
#ifdef	__SUNPRO_C
#pragma	pipeloop(0)
#endif
			for (i = 0; i < n >> 4; i++) {
				dx3 = _mm_load_si128(psrc + i);
				dx1 = _mm_unpacklo_epi8(dx3, fzero);
				dx3 = _mm_unpackhi_epi8(dx3, fzero);
				dx0 = _mm_unpacklo_epi16(dx1, fzero);
				dx1 = _mm_unpackhi_epi16(dx1, fzero);
				dx2 = _mm_unpacklo_epi16(dx3, fzero);
				dx3 = _mm_unpackhi_epi16(dx3, fzero);
				dx0 = _mm_srli_epi32(dx0, -ashift);
				dx1 = _mm_srli_epi32(dx1, -ashift);
				dx2 = _mm_srli_epi32(dx2, -ashift);
				dx3 = _mm_srli_epi32(dx3, -ashift);
				_mm_store_si128(pdst + 4 * i, dx0);
				_mm_store_si128(pdst + 4 * i + 1, dx1);
				_mm_store_si128(pdst + 4 * i + 2, dx2);
				_mm_store_si128(pdst + 4 * i + 3, dx3);
			}
		} else if (ashift <= 23) {
#ifdef	__SUNPRO_C
#pragma	pipeloop(0)
#endif
			for (i = 0; i < n >> 4; i++) {
				dx3 = _mm_load_si128(psrc + i);
				dx1 = _mm_unpacklo_epi8(dx3, fzero);
				dx3 = _mm_unpackhi_epi8(dx3, fzero);
				dx0 = _mm_unpacklo_epi16(dx1, fzero);
				dx1 = _mm_unpackhi_epi16(dx1, fzero);
				dx2 = _mm_unpacklo_epi16(dx3, fzero);
				dx3 = _mm_unpackhi_epi16(dx3, fzero);
				dx0 = _mm_slli_epi32(dx0, ashift);
				dx1 = _mm_slli_epi32(dx1, ashift);
				dx2 = _mm_slli_epi32(dx2, ashift);
				dx3 = _mm_slli_epi32(dx3, ashift);
				_mm_store_si128(pdst + 4 * i, dx0);
				_mm_store_si128(pdst + 4 * i + 1, dx1);
				_mm_store_si128(pdst + 4 * i + 2, dx2);
				_mm_store_si128(pdst + 4 * i + 3, dx3);
			}
		} else {
			__m128i mask0, mask1, mask2, mask3;
			__m128i ceiling = _mm_set_epi32(0, 0x7fffffff,
					0, 0x7fffffff);
			__m128i floor = _mm_cmpeq_epi32(floor, floor);
			__m128i mask = _mm_slli_epi64(floor, 32);
			floor = _mm_slli_epi64(floor, 31);
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			for (i = 0; i < n >> 4; i++) {
				dx3 = _mm_load_si128(psrc + i);
				dx1 = _mm_unpacklo_epi8(dx3, fzero);
				dx3 = _mm_unpackhi_epi8(dx3, fzero);
				dx0 = _mm_unpacklo_epi16(dx1, fzero);
				dx1 = _mm_unpackhi_epi16(dx1, fzero);
				dx2 = _mm_unpacklo_epi16(dx3, fzero);
				dx3 = _mm_unpackhi_epi16(dx3, fzero);
				dy0 = _mm_unpackhi_epi32(dx0, fzero);
				dx0 = _mm_unpacklo_epi32(dx0, fzero);
				dy1 = _mm_unpackhi_epi32(dx1, fzero);
				dx1 = _mm_unpacklo_epi32(dx1, fzero);
				dy2 = _mm_unpackhi_epi32(dx2, fzero);
				dx2 = _mm_unpacklo_epi32(dx2, fzero);
				dy3 = _mm_unpackhi_epi32(dx3, fzero);
				dx3 = _mm_unpacklo_epi32(dx3, fzero);
				dy0 = _mm_slli_epi64(dy0, ashift);
				dx0 = _mm_slli_epi64(dx0, ashift);
				dy1 = _mm_slli_epi64(dy1, ashift);
				dx1 = _mm_slli_epi64(dx1, ashift);
				dy2 = _mm_slli_epi64(dy2, ashift);
				dx2 = _mm_slli_epi64(dx2, ashift);
				dy3 = _mm_slli_epi64(dy3, ashift);
				dx3 = _mm_slli_epi64(dx3, ashift);

				SAT_S32_P(dy0);
				SAT_S32_P(dx0);
				SAT_S32_P(dy1);
				SAT_S32_P(dx1);
				SAT_S32_P(dy2);
				SAT_S32_P(dx2);
				SAT_S32_P(dy3);
				SAT_S32_P(dx3);

				mask0 = _mm_unpacklo_epi32(dx0, dy0);
				dx0 = _mm_unpackhi_epi32(dx0, dy0);
				dy0 = _mm_unpacklo_epi32(mask0, dx0);
				mask1 = _mm_unpacklo_epi32(dx1, dy1);
				dx1 = _mm_unpackhi_epi32(dx1, dy1);
				dy1 = _mm_unpacklo_epi32(mask1, dx1);
				mask2 = _mm_unpacklo_epi32(dx2, dy2);
				dx2 = _mm_unpackhi_epi32(dx2, dy2);
				dy2 = _mm_unpacklo_epi32(mask2, dx2);
				mask3 = _mm_unpacklo_epi32(dx3, dy3);
				dx3 = _mm_unpackhi_epi32(dx3, dy3);
				dy3 = _mm_unpacklo_epi32(mask3, dx3);

				_mm_store_si128(pdst + 4 * i, dy0);
				_mm_store_si128(pdst + 4 * i + 1, dy1);
				_mm_store_si128(pdst + 4 * i + 2, dy2);
				_mm_store_si128(pdst + 4 * i + 3, dy3);
			}
		}
	} else {
		if (ashift < 0) {
#ifdef	__SUNPRO_C
#pragma	pipeloop(0)
#endif
			for (i = 0; i < n >> 4; i++) {
				dx3 = _mm_loadu_si128(psrc + i);
				dx1 = _mm_unpacklo_epi8(dx3, fzero);
				dx3 = _mm_unpackhi_epi8(dx3, fzero);
				dx0 = _mm_unpacklo_epi16(dx1, fzero);
				dx1 = _mm_unpackhi_epi16(dx1, fzero);
				dx2 = _mm_unpacklo_epi16(dx3, fzero);
				dx3 = _mm_unpackhi_epi16(dx3, fzero);
				dx0 = _mm_srli_epi32(dx0, -ashift);
				dx1 = _mm_srli_epi32(dx1, -ashift);
				dx2 = _mm_srli_epi32(dx2, -ashift);
				dx3 = _mm_srli_epi32(dx3, -ashift);
				_mm_store_si128(pdst + 4 * i, dx0);
				_mm_store_si128(pdst + 4 * i + 1, dx1);
				_mm_store_si128(pdst + 4 * i + 2, dx2);
				_mm_store_si128(pdst + 4 * i + 3, dx3);
			}
		} else if (ashift <= 23) {
#ifdef	__SUNPRO_C
#pragma	pipeloop(0)
#endif
			for (i = 0; i < n >> 4; i++) {
				dx3 = _mm_loadu_si128(psrc + i);
				dx1 = _mm_unpacklo_epi8(dx3, fzero);
				dx3 = _mm_unpackhi_epi8(dx3, fzero);
				dx0 = _mm_unpacklo_epi16(dx1, fzero);
				dx1 = _mm_unpackhi_epi16(dx1, fzero);
				dx2 = _mm_unpacklo_epi16(dx3, fzero);
				dx3 = _mm_unpackhi_epi16(dx3, fzero);
				dx0 = _mm_slli_epi32(dx0, ashift);
				dx1 = _mm_slli_epi32(dx1, ashift);
				dx2 = _mm_slli_epi32(dx2, ashift);
				dx3 = _mm_slli_epi32(dx3, ashift);
				_mm_store_si128(pdst + 4 * i, dx0);
				_mm_store_si128(pdst + 4 * i + 1, dx1);
				_mm_store_si128(pdst + 4 * i + 2, dx2);
				_mm_store_si128(pdst + 4 * i + 3, dx3);
			}
		} else {
			__m128i mask0, mask1, mask2, mask3;
			__m128i ceiling = _mm_set_epi32(0, 0x7fffffff,
					0, 0x7fffffff);
			__m128i floor = _mm_cmpeq_epi32(floor, floor);
			__m128i mask = _mm_slli_epi64(floor, 32);
			floor = _mm_slli_epi64(floor, 31);
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			for (i = 0; i < n >> 4; i++) {
				dx3 = _mm_loadu_si128(psrc + i);
				dx1 = _mm_unpacklo_epi8(dx3, fzero);
				dx3 = _mm_unpackhi_epi8(dx3, fzero);
				dx0 = _mm_unpacklo_epi16(dx1, fzero);
				dx1 = _mm_unpackhi_epi16(dx1, fzero);
				dx2 = _mm_unpacklo_epi16(dx3, fzero);
				dx3 = _mm_unpackhi_epi16(dx3, fzero);
				dy0 = _mm_unpackhi_epi32(dx0, fzero);
				dx0 = _mm_unpacklo_epi32(dx0, fzero);
				dy1 = _mm_unpackhi_epi32(dx1, fzero);
				dx1 = _mm_unpacklo_epi32(dx1, fzero);
				dy2 = _mm_unpackhi_epi32(dx2, fzero);
				dx2 = _mm_unpacklo_epi32(dx2, fzero);
				dy3 = _mm_unpackhi_epi32(dx3, fzero);
				dx3 = _mm_unpacklo_epi32(dx3, fzero);
				dy0 = _mm_slli_epi64(dy0, ashift);
				dx0 = _mm_slli_epi64(dx0, ashift);
				dy1 = _mm_slli_epi64(dy1, ashift);
				dx1 = _mm_slli_epi64(dx1, ashift);
				dy2 = _mm_slli_epi64(dy2, ashift);
				dx2 = _mm_slli_epi64(dx2, ashift);
				dy3 = _mm_slli_epi64(dy3, ashift);
				dx3 = _mm_slli_epi64(dx3, ashift);

				SAT_S32_P(dy0);
				SAT_S32_P(dx0);
				SAT_S32_P(dy1);
				SAT_S32_P(dx1);
				SAT_S32_P(dy2);
				SAT_S32_P(dx2);
				SAT_S32_P(dy3);
				SAT_S32_P(dx3);

				mask0 = _mm_unpacklo_epi32(dx0, dy0);
				dx0 = _mm_unpackhi_epi32(dx0, dy0);
				dy0 = _mm_unpacklo_epi32(mask0, dx0);
				mask1 = _mm_unpacklo_epi32(dx1, dy1);
				dx1 = _mm_unpackhi_epi32(dx1, dy1);
				dy1 = _mm_unpacklo_epi32(mask1, dx1);
				mask2 = _mm_unpacklo_epi32(dx2, dy2);
				dx2 = _mm_unpackhi_epi32(dx2, dy2);
				dy2 = _mm_unpacklo_epi32(mask2, dx2);
				mask3 = _mm_unpacklo_epi32(dx3, dy3);
				dx3 = _mm_unpackhi_epi32(dx3, dy3);
				dy3 = _mm_unpacklo_epi32(mask3, dx3);

				_mm_store_si128(pdst + 4 * i, dy0);
				_mm_store_si128(pdst + 4 * i + 1, dy1);
				_mm_store_si128(pdst + 4 * i + 2, dy2);
				_mm_store_si128(pdst + 4 * i + 3, dy3);
			}
		}
	}

	i <<= 4;

	if (i < n) {
		mlib_u8 *psrc_n = (mlib_u8 *)src;
		mlib_s32 *pdst_n = (mlib_s32 *)dst;

		if (ashift < 0) {
			for (; i < n; i++) {
				((mlib_s32 *)dst)[i] =
				    ((mlib_u8 *)src)[i] >> (-ashift);
			}
		} else if (shift <= 0) {
			for (; i < n; i++) {
				((mlib_s32 *)dst)[i] =
				    ((mlib_u8 *)src)[i] << (ashift);
			}
		} else {
			mlib_d64 d_valc = ((mlib_u32)1) << ashift;

			for (; i < n; i++) {
				FLOAT2INT_CLAMP(((mlib_s32 *)dst)[i],
				    d_valc * ((mlib_u8 *)src)[i]);
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_S32_S16_Sat(
    mlib_s32 *dst,
    const mlib_s16 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	__m128i dx0, dx1, dx2, dx3, dy0, dy1, dy2, dy3;
	__m128i *psrc;
	__m128i *pdst;
	mlib_s32 i;
	mlib_s32 ashift;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	if (shift > 15)
		shift = 15;

	if (shift < -32)
		shift = -32;
	ashift = shift + 16;

	if (n < 8 || (mlib_addr)dst & 15) {
		mlib_s32 count =
			((n < 8) ? n : (15 & (16 - (mlib_addr)dst)) >> 2);

		if (ashift < 0)
			for (i = 0; i < count; ++i)
				((mlib_s32 *)dst)[i] =
				    ((mlib_s16 *)src)[i] >> (-ashift);
		else if (shift <= 0)
			for (i = 0; i < count; ++i)
				((mlib_s32 *)dst)[i] =
				    ((mlib_s16 *)src)[i] << (ashift);
		else {
			mlib_d64 d_valc = ((mlib_u32)1) << ashift;

			for (i = 0; i < count; ++i) {
				FLOAT2INT_CLAMP(((mlib_s32 *)dst)[i],
				    d_valc * ((mlib_s16 *)src)[i]);
			}
		}

		n -= count;
		src = (void *)(((mlib_s16 *)src) + count);
		dst = (void *)(((mlib_s32 *)dst) + count);
	}

/* pdst is 128bit-aligned now */
	psrc = (__m128i *)src;
	pdst = (__m128i *)dst;

	if (!(15 & (mlib_addr)psrc)) {
		if (ashift <= 16) {
#ifdef	__SUNPRO_C
#pragma	pipeloop(0)
#endif
			for (i = 0; i < n >> 3; i++) {
				dx1 = _mm_load_si128(psrc + i);
				dx0 = _mm_unpacklo_epi16(dx1, dx1);
				dx1 = _mm_unpackhi_epi16(dx1, dx1);
				dx0 = _mm_slli_epi32(dx0, 16);
				dx1 = _mm_slli_epi32(dx1, 16);
				dx0 = _mm_srai_epi32(dx0, 16 - ashift);
				dx1 = _mm_srai_epi32(dx1, 16 - ashift);
				_mm_store_si128(pdst + 2 * i, dx0);
				_mm_store_si128(pdst + 2 * i + 1, dx1);
			}
		} else {
			__m128i mask0, mask1, mask2, mask3;
			__m128i ceiling = _mm_set_epi32(0, 0x7fffffff,
					0, 0x7fffffff);
			__m128i floor = _mm_cmpeq_epi32(floor, floor);
			__m128i mask = _mm_slli_epi64(floor, 32);
			floor = _mm_slli_epi64(floor, 31);
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			for (i = 0; i < n >> 3; i++) {
				dx3 = _mm_load_si128(psrc + i);
				dx1 = _mm_unpacklo_epi16(dx3, dx3);
				dx3 = _mm_unpackhi_epi16(dx3, dx3);
				dx1 = _mm_slli_epi32(dx1, 16);
				dx3 = _mm_slli_epi32(dx3, 16);
				mask1 = _mm_srai_epi32(dx1, 31);
				mask3 = _mm_srai_epi32(dx3, 31);
				dy1 = _mm_unpackhi_epi32(dx1, mask1);
				dx1 = _mm_unpacklo_epi32(dx1, mask1);
				dy3 = _mm_unpackhi_epi32(dx3, mask3);
				dx3 = _mm_unpacklo_epi32(dx3, mask3);
				dy1 = _mm_slli_epi64(dy1, ashift - 16);
				dx1 = _mm_slli_epi64(dx1, ashift - 16);
				dy3 = _mm_slli_epi64(dy3, ashift - 16);
				dx3 = _mm_slli_epi64(dx3, ashift - 16);

				SAT_S32_P(dy1);
				SAT_S32_P(dx1);
				SAT_S32_P(dy3);
				SAT_S32_P(dx3);

				mask1 = _mm_unpacklo_epi32(dx1, dy1);
				dx1 = _mm_unpackhi_epi32(dx1, dy1);
				dy1 = _mm_unpacklo_epi32(mask1, dx1);
				mask3 = _mm_unpacklo_epi32(dx3, dy3);
				dx3 = _mm_unpackhi_epi32(dx3, dy3);
				dy3 = _mm_unpacklo_epi32(mask3, dx3);

				_mm_store_si128(pdst + 2 * i, dy1);
				_mm_store_si128(pdst + 2 * i + 1, dy3);
			}
		}
	} else {
		if (ashift <= 16) {
#ifdef	__SUNPRO_C
#pragma	pipeloop(0)
#endif
			for (i = 0; i < n >> 3; i++) {
				dx1 = _mm_loadu_si128(psrc + i);
				dx0 = _mm_unpacklo_epi16(dx1, dx1);
				dx1 = _mm_unpackhi_epi16(dx1, dx1);
				dx0 = _mm_slli_epi32(dx0, 16);
				dx1 = _mm_slli_epi32(dx1, 16);
				dx0 = _mm_srai_epi32(dx0, 16 - ashift);
				dx1 = _mm_srai_epi32(dx1, 16 - ashift);
				_mm_store_si128(pdst + 2 * i, dx0);
				_mm_store_si128(pdst + 2 * i + 1, dx1);
			}
		} else {
			__m128i mask0, mask1, mask2, mask3;
			__m128i ceiling = _mm_set_epi32(0, 0x7fffffff,
					0, 0x7fffffff);
			__m128i floor = _mm_cmpeq_epi32(floor, floor);
			__m128i mask = _mm_slli_epi64(floor, 32);
			floor = _mm_slli_epi64(floor, 31);
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			for (i = 0; i < n >> 3; i++) {
				dx3 = _mm_loadu_si128(psrc + i);
				dx1 = _mm_unpacklo_epi16(dx3, dx3);
				dx3 = _mm_unpackhi_epi16(dx3, dx3);
				dx1 = _mm_slli_epi32(dx1, 16);
				dx3 = _mm_slli_epi32(dx3, 16);
				mask1 = _mm_srai_epi32(dx1, 31);
				mask3 = _mm_srai_epi32(dx3, 31);
				dy1 = _mm_unpackhi_epi32(dx1, mask1);
				dx1 = _mm_unpacklo_epi32(dx1, mask1);
				dy3 = _mm_unpackhi_epi32(dx3, mask3);
				dx3 = _mm_unpacklo_epi32(dx3, mask3);
				dy1 = _mm_slli_epi64(dy1, ashift - 16);
				dx1 = _mm_slli_epi64(dx1, ashift - 16);
				dy3 = _mm_slli_epi64(dy3, ashift - 16);
				dx3 = _mm_slli_epi64(dx3, ashift - 16);

				SAT_S32_P(dy1);
				SAT_S32_P(dx1);
				SAT_S32_P(dy3);
				SAT_S32_P(dx3);

				mask1 = _mm_unpacklo_epi32(dx1, dy1);
				dx1 = _mm_unpackhi_epi32(dx1, dy1);
				dy1 = _mm_unpacklo_epi32(mask1, dx1);
				mask3 = _mm_unpacklo_epi32(dx3, dy3);
				dx3 = _mm_unpackhi_epi32(dx3, dy3);
				dy3 = _mm_unpacklo_epi32(mask3, dx3);

				_mm_store_si128(pdst + 2 * i, dy1);
				_mm_store_si128(pdst + 2 * i + 1, dy3);
			}
		}
	}

	i <<= 3;

	if (i < n) {
		mlib_s16 *psrc_n = (mlib_s16 *)psrc;
		mlib_s32 *pdst_n = (mlib_s32 *)pdst;

		if (ashift < 0) {
			for (; i < n; i++) {
				pdst_n[i] = psrc_n[i] >> (-ashift);
			}
		} else if (shift <= 0) {
			for (; i < n; i++) {
				pdst_n[i] = psrc_n[i] << ashift;
			}
		} else {
			mlib_d64 d_valc = ((mlib_u32)1) << ashift;

			for (; i < n; i++) {
				FLOAT2INT_CLAMP(pdst_n[i], d_valc * psrc_n[i]);
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
