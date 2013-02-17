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

#pragma ident	"@(#)mlib_s_SignalConvertShift_U8.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalConvertShift_[U8_S32|U8S_S32S]_Sat,
 *      mlib_SignalConvertShift_[U8_S16|U8S_S16S]_Sat,
 *      mlib_SignalConvertShift_[U8_S8|U8S_S8S]_Sat,
 *
 * SYNOPSIS
 *      mlib_status mlib_SignalConvertShift_U8S_S16S_Sat(mlib_u8        *dst,
 *                                                       const mlib_s16 *src,
 *                                                       mlib_s32       shift,
 *                                                       mlib_s32       n);
 *      mlib_status mlib_SignalConvertShift_U8S_S32S_Sat(mlib_u8        *dst,
 *                                                       const mlib_s32 *src,
 *                                                       mlib_s32       shift,
 *                                                       mlib_s32       n);
 *      mlib_status mlib_SignalConvertShift_U8S_S8S_Sat(mlib_u8       *dst,
 *                                                      const mlib_s8 *src,
 *                                                      mlib_s32      shift,
 *                                                      mlib_s32      n);
 *      mlib_status mlib_SignalConvertShift_U8_S16_Sat(mlib_u8        *dst,
 *                                                     const mlib_s16 *src,
 *                                                     mlib_s32       shift,
 *                                                     mlib_s32       n);
 *      mlib_status mlib_SignalConvertShift_U8_S32_Sat(mlib_u8        *dst,
 *                                                     const mlib_s32 *src,
 *                                                     mlib_s32       shift,
 *                                                     mlib_s32       n);
 *      mlib_status mlib_SignalConvertShift_U8_S8_Sat(mlib_u8       *dst,
 *                                                    const mlib_s8 *src,
 *                                                    mlib_s32      shift,
 *                                                    mlib_s32      n);
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

#pragma weak mlib_SignalConvertShift_U8_S16_Sat = \
	__mlib_SignalConvertShift_U8_S16_Sat
#pragma weak mlib_SignalConvertShift_U8_S32_Sat = \
	__mlib_SignalConvertShift_U8_S32_Sat
#pragma weak mlib_SignalConvertShift_U8_S8_Sat = \
	__mlib_SignalConvertShift_U8_S8_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalConvertShift_U8_S16_Sat)
    mlib_SignalConvertShift_U8_S16_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_U8_S16_Sat")));
__typeof__(__mlib_SignalConvertShift_U8_S32_Sat)
    mlib_SignalConvertShift_U8_S32_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_U8_S32_Sat")));
__typeof__(__mlib_SignalConvertShift_U8_S8_Sat)
    mlib_SignalConvertShift_U8_S8_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_U8_S8_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	SAT_U8_INT(dst, src)                                        \
	{                                                           \
		dst = (src);                                        \
		if ((src) > 255) {                                  \
			dst = 255;                                  \
		} else if ((src) < 0) {                             \
			dst = 0;                                    \
		}                                                   \
	}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_U8_S8_Sat(
    mlib_u8 *dst,
    const mlib_s8 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_s32 i;
	__m128i *psrc;
	__m128i *pdst;
	__m128i dx, dx0, dx1, dy0, dy1;
	__m128i fzero = _mm_setzero_si128();
	mlib_s32 ashift;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	if (shift > 7)
		shift = 7;

	if (shift < -8)
		shift = -8;
	ashift = -(shift + 1);

	if (n < 16 || (mlib_addr)src & 15) {
		mlib_s32 count;
		mlib_s8 *psrc_n = (mlib_s8 *)src;
		mlib_u8 *pdst_n = (mlib_u8 *)dst;

		count = (n < 16) ? n : ((16 - (mlib_addr)src) & 15);

		if (shift < 0) {
			for (i = 0; i < count; i++) {
				mlib_s32 t = psrc_n[i];

				pdst_n[i] = (t >> ashift) & ~(t >> 7);
			}
		} else {
			for (i = 0; i < count; i++) {
				mlib_u32 t0 = ((mlib_u8 *)psrc_n)[i], t;
				mlib_s32 mask;

				t = t0 << (-ashift);
				mask = (0xff - (mlib_s32)t) >> 31;
				pdst_n[i] =
				    (t | mask) & ~((mlib_s32)(t0 << 24) >> 31);
			}
		}

		n -= count;
		dst = (void *)((mlib_u8 *)dst + count);
		src = (void *)((mlib_s8 *)src + count);
	}

/* psrc is 128bits-aligned now */

	psrc = (__m128i *)src;
	pdst = (__m128i *)dst;

	if (!(15 & (mlib_addr)pdst)) {
		if (shift < 0) {
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			for (i = 0; i < n >> 4; i++) {
				dx0 = _mm_load_si128(psrc + i);
				dy0 = _mm_unpacklo_epi8(dx0, fzero);
				dy1 = _mm_unpackhi_epi8(dx0, fzero);
				dy0 = _mm_slli_epi16(dy0, 8);
				dy1 = _mm_slli_epi16(dy1, 8);
				dy0 = _mm_srai_epi16(dy0, ashift + 8);
				dy1 = _mm_srai_epi16(dy1, ashift + 8);
				dx = _mm_packus_epi16(dy0, dy1);
				_mm_store_si128(pdst + i, dx);
			}
		} else {
			__m128i floor = _mm_setzero_si128();
			__m128i ceiling = _mm_set1_epi32(0x00ff00ff);
			__m128i mask;
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			for (i = 0; i < n >> 4; i++) {
				dx0 = _mm_load_si128(psrc + i);
				dy0 = _mm_unpacklo_epi8(dx0, fzero);
				dy1 = _mm_unpackhi_epi8(dx0, fzero);
				dy0 = _mm_slli_epi16(dy0, 8);
				dy1 = _mm_slli_epi16(dy1, 8);
				dy0 = _mm_srai_epi16(dy0, ashift + 8);
				dy1 = _mm_srai_epi16(dy1, ashift + 8);
				dy0 = _mm_min_epi16(dy0, ceiling);
				dy1 = _mm_min_epi16(dy1, ceiling);
				dy0 = _mm_max_epi16(dy0, floor);
				dy1 = _mm_max_epi16(dy1, floor);
				dx = _mm_packus_epi16(dy0, dy1);
				_mm_store_si128(pdst + i, dx);

			}
		}
	} else {
		if (shift < 0) {
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			for (i = 0; i < n >> 4; i++) {
				dx0 = _mm_load_si128(psrc + i);
				dy0 = _mm_unpacklo_epi8(dx0, fzero);
				dy1 = _mm_unpackhi_epi8(dx0, fzero);
				dy0 = _mm_slli_epi16(dy0, 8);
				dy1 = _mm_slli_epi16(dy1, 8);
				dy0 = _mm_srai_epi16(dy0, 8 + ashift);
				dy1 = _mm_srai_epi16(dy1, 8 + ashift);
				dx = _mm_packus_epi16(dy0, dy1);
				_mm_storeu_si128(pdst + i, dx);
			}
		} else {
			__m128i floor = _mm_setzero_si128();
			__m128i ceiling = _mm_set1_epi32(0x00ff00ff);
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			for (i = 0; i < n >> 4; i++) {
				dx0 = _mm_load_si128(psrc + i);
				dy0 = _mm_unpacklo_epi8(dx0, fzero);
				dy1 = _mm_unpackhi_epi8(dx0, fzero);
				dy0 = _mm_slli_epi16(dy0, 8);
				dy1 = _mm_slli_epi16(dy1, 8);
				dy0 = _mm_srai_epi16(dy0, ashift + 8);
				dy1 = _mm_srai_epi16(dy1, ashift + 8);
				dy0 = _mm_min_epi16(dy0, ceiling);
				dy1 = _mm_min_epi16(dy1, ceiling);
				dy0 = _mm_max_epi16(dy0, floor);
				dy1 = _mm_max_epi16(dy1, floor);
				dx = _mm_packus_epi16(dy0, dy1);
				_mm_storeu_si128(pdst + i, dx);
			}
		}
	}

	i <<= 4;

	if (i < n) {
		mlib_s8 *psrc_n = (mlib_s8 *)psrc;
		mlib_u8 *pdst_n = (mlib_u8 *)pdst;

		if (shift < 0) {
			for (; i < n; i++) {
				mlib_s32 t = psrc_n[i];

				pdst_n[i] = (t >> ashift) & ~(t >> 7);
			}
		} else {
			for (; i < n; i++) {
				mlib_u32 t0 = ((mlib_u8 *)psrc_n)[i], t;
				mlib_s32 mask;

				t = t0 << (-ashift);
				mask = (0xff - (mlib_s32)t) >> 31;
				pdst_n[i] =
				    (t | mask) & ~((mlib_s32)(t0 << 24) >> 31);
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_U8_S16_Sat(
    mlib_u8 *dst,
    const mlib_s16 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_d64 d_val;
	__m128i *psrc;
	__m128i *pdst;
	__m128i dx, dx0, dx1, dy0, dy1, dy2, dy3;
	mlib_s32 ashift;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	if (shift > 15)
		shift = 15;

	if (shift < -8)
		shift = -8;
	ashift = 7 - shift;

	if (n < 8 || (mlib_addr)src & 15) {
		mlib_s32 count;
		mlib_s16 *psrc_n = (mlib_s16 *)src;
		mlib_u8 *pdst_n = (mlib_u8 *)dst;

		count = (n < 8) ? n : (((16 - (mlib_addr)src) & 15) >> 1);

		if (ashift >= 7) {
			for (i = 0; i < count; i++) {
				mlib_s32 t = psrc_n[i];
				pdst_n[i] = (t >> ashift) & ~(t >> 15);
			}
		} else if (ashift >= 0) {
			for (i = 0; i < count; i++) {
				SAT_U8_INT(pdst_n[i],
					(mlib_s32)psrc_n[i] >> ashift);
			}
		} else {
			for (i = 0; i < count; i++) {
				SAT_U8_INT(pdst_n[i],
					(mlib_s32)psrc_n[i] << (-ashift));
			}
		}

		n -= count;
		dst = (void *)((mlib_u8 *)dst + count);
		src = (void *)((mlib_s16 *)src + count);
	}

/* pdst is d64-aligned now */
/* psrc is 128bits-aligned now */

	psrc = (__m128i *)src;
	pdst = (__m128i *)dst;

	if (!(15 & (mlib_addr)pdst)) {
		if (ashift >= 0) {
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			for (i = 0; i < n >> 4; i++) {
				dy0 = _mm_load_si128(psrc + 2 * i);
				dy1 = _mm_load_si128(psrc + 2 * i + 1);
				dy0 = _mm_srai_epi16(dy0, ashift);
				dy1 = _mm_srai_epi16(dy1, ashift);
				dx = _mm_packus_epi16(dy0, dy1);
				_mm_store_si128(pdst + i, dx);
			}
		} else {
			__m128i mask0, mask1;
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			for (i = 0; i < n >> 4; i++) {
				dy0 = _mm_load_si128(psrc + 2 * i);
				dy1 = _mm_load_si128(psrc + 2 * i + 1);
				mask0 = _mm_srai_epi16(dy0, 15);
				mask1 = _mm_srai_epi16(dy1, 15);
				dy2 = _mm_unpacklo_epi16(dy0, mask0);
				dy3 = _mm_unpacklo_epi16(dy1, mask1);
				dy0 = _mm_unpackhi_epi16(dy0, mask0);
				dy1 = _mm_unpackhi_epi16(dy1, mask1);
				dy2 = _mm_slli_epi32(dy2, -ashift);
				dy3 = _mm_slli_epi32(dy3, -ashift);
				dy0 = _mm_slli_epi32(dy0, -ashift);
				dy1 = _mm_slli_epi32(dy1, -ashift);
				dy0 = _mm_packs_epi32(dy2, dy0);
				dy1 = _mm_packs_epi32(dy3, dy1);
				dx = _mm_packus_epi16(dy0, dy1);
				_mm_store_si128(pdst + i, dx);
			}
		}
	} else {
		if (ashift >= 0) {
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			for (i = 0; i < n >> 4; i++) {
				dy0 = _mm_load_si128(psrc + 2 * i);
				dy1 = _mm_load_si128(psrc + 2 * i + 1);
				dy0 = _mm_srai_epi16(dy0, ashift);
				dy1 = _mm_srai_epi16(dy1, ashift);
				dx = _mm_packus_epi16(dy0, dy1);
				_mm_storeu_si128(pdst + i, dx);
			}
		} else {
			__m128i mask0, mask1;
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			for (i = 0; i < n >> 4; i++) {
				dy0 = _mm_load_si128(psrc + 2 * i);
				dy1 = _mm_load_si128(psrc + 2 * i + 1);
				mask0 = _mm_srai_epi16(dy0, 15);
				mask1 = _mm_srai_epi16(dy1, 15);
				dy2 = _mm_unpacklo_epi16(dy0, mask0);
				dy3 = _mm_unpacklo_epi16(dy1, mask1);
				dy0 = _mm_unpackhi_epi16(dy0, mask0);
				dy1 = _mm_unpackhi_epi16(dy1, mask1);
				dy2 = _mm_slli_epi32(dy2, -ashift);
				dy3 = _mm_slli_epi32(dy3, -ashift);
				dy0 = _mm_slli_epi32(dy0, -ashift);
				dy1 = _mm_slli_epi32(dy1, -ashift);
				dy0 = _mm_packs_epi32(dy2, dy0);
				dy1 = _mm_packs_epi32(dy3, dy1);
				dx = _mm_packus_epi16(dy0, dy1);
				_mm_storeu_si128(pdst + i, dx);
			}
		}
	}

	i <<= 4;

	if (i < n) {
		mlib_s16 *psrc_n = (mlib_s16 *)psrc;
		mlib_u8 *pdst_n = (mlib_u8 *)pdst;

		if (ashift >= 7) {
			for (; i < n; i++) {
				mlib_s32 t = psrc_n[i];
				pdst_n[i] = (t >> ashift) & ~(t >> 15);
			}
		} else if (ashift >= 0) {
			for (; i < n; i++) {
				SAT_U8_INT(pdst_n[i],
					(mlib_s32)psrc_n[i] >> ashift);
			}
		} else {
			for (; i < n; i++) {
				SAT_U8_INT(pdst_n[i],
					(mlib_s32)psrc_n[i] << (-ashift));
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	SAT_S8_P(x)                                                 \
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
__mlib_SignalConvertShift_U8_S32_Sat(
    mlib_u8 *dst,
    const mlib_s32 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_d64 d_val;
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

	if (shift < -8)
		shift = -8;
	ashift = 23 - shift;

	if (n < 4 || (mlib_addr)src & 15) {
		mlib_s32 count;
		mlib_d64 d_val;
		mlib_s32 *psrc_n = (mlib_s32 *)src;
		mlib_u8 *pdst_n = (mlib_u8 *)dst;

		count = (n < 4) ? n : (((16 - (mlib_addr)src) & 15) >> 2);

		if (shift <= 0) {
			for (i = 0; i < count; i++) {
				mlib_s32 t = psrc_n[i];

				pdst_n[i] = (t >> ashift) & ~(t >> 31);
			}
		} else {
			d_val = 2.0 * (((mlib_u32)1) << shift);
			for (i = 0; i < count; i++) {
				SAT_U8(pdst_n[i], d_val * psrc_n[i]);
			}
		}

		if (n < 4)
			return (MLIB_SUCCESS);
		n -= count;
		dst = (void *)((mlib_u8 *)dst + count);
		src = (void *)((mlib_s32 *)src + count);
	}

/* pdst is d64-aligned now */
/* psrc is 128bits-aligned now */

	psrc = (__m128i *)src;
	pdst = (__m128i *)dst;

	if (!(15 & (mlib_addr)pdst)) {

		if (ashift >= 0) {
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			for (i = 0; i < n >> 4; i++) {
				dy0 = _mm_load_si128(psrc + 4 * i);
				dy1 = _mm_load_si128(psrc + 4 * i + 1);
				dy2 = _mm_load_si128(psrc + 4 * i + 2);
				dy3 = _mm_load_si128(psrc + 4 * i + 3);
				dy0 = _mm_srai_epi32(dy0, ashift);
				dy1 = _mm_srai_epi32(dy1, ashift);
				dy2 = _mm_srai_epi32(dy2, ashift);
				dy3 = _mm_srai_epi32(dy3, ashift);
				dy0 = _mm_packs_epi32(dy0, dy1);
				dy2 = _mm_packs_epi32(dy2, dy3);
				dx = _mm_packus_epi16(dy0, dy2);
				_mm_store_si128(pdst + i, dx);
			}
		} else {
			__m128i dy4, dy5, dy6, dy7;
			__m128i mask0, mask1, mask2, mask3;
			__m128i ceiling = _mm_set_epi32(0, 0xff, 0, 0xff);
			__m128i floor = _mm_setzero_si128();
			__m128i mask = _mm_cmpeq_epi32(mask, mask);
			mask = _mm_slli_epi64(mask, 32);
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			for (i = 0; i < n >> 4; i++) {
				dy0 = _mm_load_si128(psrc + 4 * i);
				dy1 = _mm_load_si128(psrc + 4 * i + 1);
				dy2 = _mm_load_si128(psrc + 4 * i + 2);
				dy3 = _mm_load_si128(psrc + 4 * i + 3);
				mask0 = _mm_srai_epi32(dy0, 31);
				mask1 = _mm_srai_epi32(dy1, 31);
				mask2 = _mm_srai_epi32(dy2, 31);
				mask3 = _mm_srai_epi32(dy3, 31);
				dy4 = _mm_unpackhi_epi32(dy0, mask0);
				dy0 = _mm_unpacklo_epi32(dy0, mask0);
				dy5 = _mm_unpackhi_epi32(dy1, mask1);
				dy1 = _mm_unpacklo_epi32(dy1, mask1);
				dy6 = _mm_unpackhi_epi32(dy2, mask2);
				dy2 = _mm_unpacklo_epi32(dy2, mask2);
				dy7 = _mm_unpackhi_epi32(dy3, mask3);
				dy3 = _mm_unpacklo_epi32(dy3, mask3);
				dy4 = _mm_slli_epi64(dy4, -ashift);
				dy0 = _mm_slli_epi64(dy0, -ashift);
				dy5 = _mm_slli_epi64(dy5, -ashift);
				dy1 = _mm_slli_epi64(dy1, -ashift);
				dy6 = _mm_slli_epi64(dy6, -ashift);
				dy2 = _mm_slli_epi64(dy2, -ashift);
				dy7 = _mm_slli_epi64(dy7, -ashift);
				dy3 = _mm_slli_epi64(dy3, -ashift);

				SAT_S8_P(dy4);
				SAT_S8_P(dy0);
				SAT_S8_P(dy5);
				SAT_S8_P(dy1);
				SAT_S8_P(dy6);
				SAT_S8_P(dy2);
				SAT_S8_P(dy7);
				SAT_S8_P(dy3);

				dx = _mm_unpacklo_epi8(dy0, dy2);
				dy2 = _mm_unpackhi_epi8(dy0, dy2);
				dy0 = dx;
				dx = _mm_unpacklo_epi8(dy1, dy3);
				dy3 = _mm_unpackhi_epi8(dy1, dy3);
				dy1 = dx;
				dx = _mm_unpacklo_epi8(dy4, dy6);
				dy6 = _mm_unpackhi_epi8(dy4, dy6);
				dy4 = dx;
				dx = _mm_unpacklo_epi8(dy5, dy7);
				dy7 = _mm_unpackhi_epi8(dy5, dy7);
				dy5 = dx;

				dy0 = _mm_unpacklo_epi8(dy0, dy1);
				dy4 = _mm_unpacklo_epi8(dy4, dy5);
				dy2 = _mm_unpacklo_epi8(dy2, dy3);
				dy6 = _mm_unpacklo_epi8(dy6, dy7);

				dy0 = _mm_unpacklo_epi8(dy0, dy4);
				dy2 = _mm_unpacklo_epi8(dy2, dy6);

				dy0 = _mm_unpacklo_epi8(dy0, dy2);
				_mm_store_si128(pdst + i, dy0);
			}
		}
	} else {
		if (ashift >= 0) {
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			for (i = 0; i < n >> 4; i++) {
				dy0 = _mm_load_si128(psrc + 4 * i);
				dy1 = _mm_load_si128(psrc + 4 * i + 1);
				dy2 = _mm_load_si128(psrc + 4 * i + 2);
				dy3 = _mm_load_si128(psrc + 4 * i + 3);
				dy0 = _mm_srai_epi32(dy0, ashift);
				dy1 = _mm_srai_epi32(dy1, ashift);
				dy2 = _mm_srai_epi32(dy2, ashift);
				dy3 = _mm_srai_epi32(dy3, ashift);
				dy0 = _mm_packs_epi32(dy0, dy1);
				dy2 = _mm_packs_epi32(dy2, dy3);
				dx = _mm_packus_epi16(dy0, dy2);
				_mm_storeu_si128(pdst + i, dx);
			}
		} else {
			__m128i dy4, dy5, dy6, dy7;
			__m128i mask0, mask1, mask2, mask3;
			__m128i ceiling = _mm_set_epi32(0, 0xff, 0, 0xff);
			__m128i floor = _mm_setzero_si128();
			__m128i mask = _mm_cmpeq_epi32(mask, mask);
			mask = _mm_slli_epi64(mask, 32);
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			for (i = 0; i < n >> 4; i++) {
				dy0 = _mm_load_si128(psrc + 4 * i);
				dy1 = _mm_load_si128(psrc + 4 * i + 1);
				dy2 = _mm_load_si128(psrc + 4 * i + 2);
				dy3 = _mm_load_si128(psrc + 4 * i + 3);
				mask0 = _mm_srai_epi32(dy0, 31);
				mask1 = _mm_srai_epi32(dy1, 31);
				mask2 = _mm_srai_epi32(dy2, 31);
				mask3 = _mm_srai_epi32(dy3, 31);
				dy4 = _mm_unpackhi_epi32(dy0, mask0);
				dy0 = _mm_unpacklo_epi32(dy0, mask0);
				dy5 = _mm_unpackhi_epi32(dy1, mask1);
				dy1 = _mm_unpacklo_epi32(dy1, mask1);
				dy6 = _mm_unpackhi_epi32(dy2, mask2);
				dy2 = _mm_unpacklo_epi32(dy2, mask2);
				dy7 = _mm_unpackhi_epi32(dy3, mask3);
				dy3 = _mm_unpacklo_epi32(dy3, mask3);
				dy4 = _mm_slli_epi64(dy4, -ashift);
				dy0 = _mm_slli_epi64(dy0, -ashift);
				dy5 = _mm_slli_epi64(dy5, -ashift);
				dy1 = _mm_slli_epi64(dy1, -ashift);
				dy6 = _mm_slli_epi64(dy6, -ashift);
				dy2 = _mm_slli_epi64(dy2, -ashift);
				dy7 = _mm_slli_epi64(dy7, -ashift);
				dy3 = _mm_slli_epi64(dy3, -ashift);

				SAT_S8_P(dy4);
				SAT_S8_P(dy0);
				SAT_S8_P(dy5);
				SAT_S8_P(dy1);
				SAT_S8_P(dy6);
				SAT_S8_P(dy2);
				SAT_S8_P(dy7);
				SAT_S8_P(dy3);

				dx = _mm_unpacklo_epi8(dy0, dy2);
				dy2 = _mm_unpackhi_epi8(dy0, dy2);
				dy0 = dx;
				dx = _mm_unpacklo_epi8(dy1, dy3);
				dy3 = _mm_unpackhi_epi8(dy1, dy3);
				dy1 = dx;
				dx = _mm_unpacklo_epi8(dy4, dy6);
				dy6 = _mm_unpackhi_epi8(dy4, dy6);
				dy4 = dx;
				dx = _mm_unpacklo_epi8(dy5, dy7);
				dy7 = _mm_unpackhi_epi8(dy5, dy7);
				dy5 = dx;

				dy0 = _mm_unpacklo_epi8(dy0, dy1);
				dy4 = _mm_unpacklo_epi8(dy4, dy5);
				dy2 = _mm_unpacklo_epi8(dy2, dy3);
				dy6 = _mm_unpacklo_epi8(dy6, dy7);

				dy0 = _mm_unpacklo_epi8(dy0, dy4);
				dy2 = _mm_unpacklo_epi8(dy2, dy6);

				dy0 = _mm_unpacklo_epi8(dy0, dy2);
				_mm_storeu_si128(pdst + i, dy0);
			}
		}
	}

	i <<= 4;

	if (i < n) {
		mlib_s32 *psrc_n = (mlib_s32 *)psrc;
		mlib_u8 *pdst_n = (mlib_u8 *)pdst;

		if (shift <= 0) {
			for (; i < n; i++) {
				mlib_s32 t = psrc_n[i];

				pdst_n[i] = (t >> ashift) & ~(t >> 31);
			}
		} else {
			d_val = 2.0 * (((mlib_u32)1) << shift);
			for (; i < n; i++) {
				SAT_U8(pdst_n[i], d_val * psrc_n[i]);
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
