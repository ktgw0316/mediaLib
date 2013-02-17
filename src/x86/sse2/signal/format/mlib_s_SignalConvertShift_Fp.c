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

#pragma ident	"@(#)mlib_s_SignalConvertShift_Fp.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *
 *      mlib_SignalConvertShift_[U8_F32|U8S_F32S]_Sat
 *      mlib_SignalConvertShift_[S8_F32|S8S_F32S]_Sat
 *      mlib_SignalConvertShift_[S16_F32|S16S_F32S]_Sat
 *
 *      mlib_SignalConvertShift_[F32_U8|F32S_U8S]
 *      mlib_SignalConvertShift_[F32_S8|F32S_S8S]
 *      mlib_SignalConvertShift_[F32_S16|F32S_S16S]
 *
 *      mlib_SignalConvertShift_[S32_F32|S32S_F32S]_Sat
 *      mlib_SignalConvertShift_[F32_S32|F32S_S32S]
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalConvertShift_F32S_S16S(mlib_f32       *dst,
 *                                                    const mlib_s16 *src,
 *                                                    mlib_s32       shift,
 *                                                    mlib_s32       n);
 *      mlib_status mlib_SignalConvertShift_F32S_S8S(mlib_f32      *dst,
 *                                                   const mlib_s8 *src,
 *                                                   mlib_s32      shift,
 *                                                   mlib_s32      n);
 *      mlib_status mlib_SignalConvertShift_F32S_U8S(mlib_f32      *dst,
 *                                                   const mlib_u8 *src,
 *                                                   mlib_s32      shift,
 *                                                   mlib_s32      n);
 *      mlib_status mlib_SignalConvertShift_F32_S16(mlib_f32       *dst,
 *                                                  const mlib_s16 *src,
 *                                                  mlib_s32       shift,
 *                                                  mlib_s32       n);
 *      mlib_status mlib_SignalConvertShift_F32_S8(mlib_f32      *dst,
 *                                                 const mlib_s8 *src,
 *                                                 mlib_s32      shift,
 *                                                 mlib_s32      n);
 *      mlib_status mlib_SignalConvertShift_F32_U8(mlib_f32      *dst,
 *                                                 const mlib_u8 *src,
 *                                                 mlib_s32      shift,
 *                                                 mlib_s32      n);
 *      mlib_status mlib_SignalConvertShift_S16S_F32S_Sat(mlib_s16       *dst,
 *                                                        const mlib_f32 *src,
 *                                                        mlib_s32       shift,
 *                                                        mlib_s32       n);
 *      mlib_status mlib_SignalConvertShift_S16_F32_Sat(mlib_s16       *dst,
 *                                                      const mlib_f32 *src,
 *                                                      mlib_s32       shift,
 *                                                      mlib_s32       n);
 *      mlib_status mlib_SignalConvertShift_S8S_F32S_Sat(mlib_s8        *dst,
 *                                                       const mlib_f32 *src,
 *                                                       mlib_s32       shift,
 *                                                       mlib_s32       n);
 *      mlib_status mlib_SignalConvertShift_S8_F32_Sat(mlib_s8        *dst,
 *                                                     const mlib_f32 *src,
 *                                                     mlib_s32       shift,
 *                                                     mlib_s32       n);
 *      mlib_status mlib_SignalConvertShift_U8S_F32S_Sat(mlib_u8        *dst,
 *                                                       const mlib_f32 *src,
 *                                                       mlib_s32       shift,
 *                                                       mlib_s32       n);
 *      mlib_status mlib_SignalConvertShift_U8_F32_Sat(mlib_u8        *dst,
 *                                                     const mlib_f32 *src,
 *                                                     mlib_s32       shift,
 *                                                     mlib_s32       n);
 *      mlib_status mlib_SignalConvertShift_F32S_S32S(mlib_f32       *dst,
 *                                                    const mlib_s32 *src,
 *                                                    mlib_s32       shift,
 *                                                    mlib_s32       n);
 *      mlib_status mlib_SignalConvertShift_F32_S32(mlib_f32       *dst,
 *                                                  const mlib_s32 *src,
 *                                                  mlib_s32       shift,
 *                                                  mlib_s32       n);
 *      mlib_status mlib_SignalConvertShift_S32S_F32S_Sat(mlib_s32       *dst,
 *                                                        const mlib_f32 *src,
 *                                                        mlib_s32       shift,
 *                                                        mlib_s32       n);
 *      mlib_status mlib_SignalConvertShift_S32_F32_Sat(mlib_s32       *dst,
 *                                                      const mlib_f32 *src,
 *                                                      mlib_s32       shift,
 *                                                      mlib_s32       n);
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

#ifdef	__SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif
#include <mlib_signal.h>
#include <mlib_SignalConvertShift.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalConvertShift_F32S_S16S = \
	__mlib_SignalConvertShift_F32S_S16S
#pragma weak mlib_SignalConvertShift_F32S_S8S = \
	__mlib_SignalConvertShift_F32S_S8S
#pragma weak mlib_SignalConvertShift_F32S_U8S = \
	__mlib_SignalConvertShift_F32S_U8S
#pragma weak mlib_SignalConvertShift_F32_S16 = \
	__mlib_SignalConvertShift_F32_S16
#pragma weak mlib_SignalConvertShift_F32_S8 = \
	__mlib_SignalConvertShift_F32_S8
#pragma weak mlib_SignalConvertShift_F32_U8 = \
	__mlib_SignalConvertShift_F32_U8
#pragma weak mlib_SignalConvertShift_S16S_F32S_Sat = \
	__mlib_SignalConvertShift_S16S_F32S_Sat
#pragma weak mlib_SignalConvertShift_S16_F32_Sat = \
	__mlib_SignalConvertShift_S16_F32_Sat
#pragma weak mlib_SignalConvertShift_S8S_F32S_Sat = \
	__mlib_SignalConvertShift_S8S_F32S_Sat
#pragma weak mlib_SignalConvertShift_S8_F32_Sat = \
	__mlib_SignalConvertShift_S8_F32_Sat
#pragma weak mlib_SignalConvertShift_U8S_F32S_Sat = \
	__mlib_SignalConvertShift_U8S_F32S_Sat
#pragma weak mlib_SignalConvertShift_U8_F32_Sat = \
	__mlib_SignalConvertShift_U8_F32_Sat
#pragma weak mlib_SignalConvertShift_F32S_S32S = \
	__mlib_SignalConvertShift_F32S_S32S
#pragma weak mlib_SignalConvertShift_F32_S32 = \
	__mlib_SignalConvertShift_F32_S32
#pragma weak mlib_SignalConvertShift_S32S_F32S_Sat = \
	__mlib_SignalConvertShift_S32S_F32S_Sat
#pragma weak mlib_SignalConvertShift_S32_F32_Sat = \
	__mlib_SignalConvertShift_S32_F32_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalConvertShift_F32S_S16S)
    mlib_SignalConvertShift_F32S_S16S
    __attribute__((weak, alias("__mlib_SignalConvertShift_F32S_S16S")));
__typeof__(__mlib_SignalConvertShift_F32S_S8S) mlib_SignalConvertShift_F32S_S8S
    __attribute__((weak, alias("__mlib_SignalConvertShift_F32S_S8S")));
__typeof__(__mlib_SignalConvertShift_F32S_U8S) mlib_SignalConvertShift_F32S_U8S
    __attribute__((weak, alias("__mlib_SignalConvertShift_F32S_U8S")));
__typeof__(__mlib_SignalConvertShift_F32_S16) mlib_SignalConvertShift_F32_S16
    __attribute__((weak, alias("__mlib_SignalConvertShift_F32_S16")));
__typeof__(__mlib_SignalConvertShift_F32_S8) mlib_SignalConvertShift_F32_S8
    __attribute__((weak, alias("__mlib_SignalConvertShift_F32_S8")));
__typeof__(__mlib_SignalConvertShift_F32_U8) mlib_SignalConvertShift_F32_U8
    __attribute__((weak, alias("__mlib_SignalConvertShift_F32_U8")));
__typeof__(__mlib_SignalConvertShift_S16S_F32S_Sat)
    mlib_SignalConvertShift_S16S_F32S_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_S16S_F32S_Sat")));
__typeof__(__mlib_SignalConvertShift_S16_F32_Sat)
    mlib_SignalConvertShift_S16_F32_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_S16_F32_Sat")));
__typeof__(__mlib_SignalConvertShift_S8S_F32S_Sat)
    mlib_SignalConvertShift_S8S_F32S_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_S8S_F32S_Sat")));
__typeof__(__mlib_SignalConvertShift_S8_F32_Sat)
    mlib_SignalConvertShift_S8_F32_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_S8_F32_Sat")));
__typeof__(__mlib_SignalConvertShift_U8S_F32S_Sat)
    mlib_SignalConvertShift_U8S_F32S_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_U8S_F32S_Sat")));
__typeof__(__mlib_SignalConvertShift_U8_F32_Sat)
    mlib_SignalConvertShift_U8_F32_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_U8_F32_Sat")));
__typeof__(__mlib_SignalConvertShift_F32S_S32S)
    mlib_SignalConvertShift_F32S_S32S
    __attribute__((weak, alias("__mlib_SignalConvertShift_F32S_S32S")));
__typeof__(__mlib_SignalConvertShift_F32_S32) mlib_SignalConvertShift_F32_S32
    __attribute__((weak, alias("__mlib_SignalConvertShift_F32_S32")));
__typeof__(__mlib_SignalConvertShift_S32S_F32S_Sat)
    mlib_SignalConvertShift_S32S_F32S_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_S32S_F32S_Sat")));
__typeof__(__mlib_SignalConvertShift_S32_F32_Sat)
    mlib_SignalConvertShift_S32_F32_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_S32_F32_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#undef	SAT_S8
#define	SAT_S8(out, in)                                              \
	{                                                            \
		mlib_f32 x = (in);                                   \
		x = x > -128.0 ? x : -128.0;                         \
		x = x < 127.0 ? x : 127.0;                           \
		out = x;                                             \
	}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_S8_F32_Sat(
    mlib_s8 *dst,
    const mlib_f32 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_s32 i, count;
	mlib_f32 *psrc = (mlib_f32 *)src;
	mlib_s8 *pdst = (mlib_s8 *)dst;
	__m128 dx0, dx1, dx2, dx3;
	__m128i dy0, dy1, dy2, dy3;
	mlib_f32 fl;
	__m128 flp;
	__m128 max, min;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	PREPARE_CONST(fl, shift + 7);

	flp = _mm_set1_ps(fl);
	max = _mm_set1_ps(127.0);
	min = _mm_set1_ps(-128.0);

	if (n < 16 || (mlib_addr)psrc & 15) {
		count = (n < 16) ? n : (((16 - (mlib_addr)psrc) & 15) >> 2);

		for (i = 0; i < count; i++) {
			SAT_S8(pdst[i], psrc[i] * fl);
		}

		n -= count;
		pdst += count;
		psrc += count;
	}

	if (!(15 & (mlib_addr)pdst)) {
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n >> 4; i++) {
			dx0 = _mm_load_ps(psrc + 16 * i);
			dx1 = _mm_load_ps(psrc + 16 * i + 4);
			dx2 = _mm_load_ps(psrc + 16 * i + 8);
			dx3 = _mm_load_ps(psrc + 16 * i + 12);
			dx0 = _mm_mul_ps(dx0, flp);
			dx1 = _mm_mul_ps(dx1, flp);
			dx2 = _mm_mul_ps(dx2, flp);
			dx3 = _mm_mul_ps(dx3, flp);
			dx0 = _mm_max_ps(dx0, min);
			dx1 = _mm_max_ps(dx1, min);
			dx2 = _mm_max_ps(dx2, min);
			dx3 = _mm_max_ps(dx3, min);
			dx0 = _mm_min_ps(dx0, max);
			dx1 = _mm_min_ps(dx1, max);
			dx2 = _mm_min_ps(dx2, max);
			dx3 = _mm_min_ps(dx3, max);
			dy0 = _mm_cvtps_epi32(dx0);
			dy1 = _mm_cvtps_epi32(dx1);
			dy2 = _mm_cvtps_epi32(dx2);
			dy3 = _mm_cvtps_epi32(dx3);
			dy0 = _mm_packs_epi32(dy0, dy1);
			dy2 = _mm_packs_epi32(dy2, dy3);
			dy0 = _mm_packs_epi16(dy0, dy2);
			_mm_store_si128((__m128i *)(pdst + 16 * i), dy0);
		}
	} else {
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n >> 4; i++) {
			dx0 = _mm_load_ps(psrc + 16 * i);
			dx1 = _mm_load_ps(psrc + 16 * i + 4);
			dx2 = _mm_load_ps(psrc + 16 * i + 8);
			dx3 = _mm_load_ps(psrc + 16 * i + 12);
			dx0 = _mm_mul_ps(dx0, flp);
			dx1 = _mm_mul_ps(dx1, flp);
			dx2 = _mm_mul_ps(dx2, flp);
			dx3 = _mm_mul_ps(dx3, flp);
			dx0 = _mm_max_ps(dx0, min);
			dx1 = _mm_max_ps(dx1, min);
			dx2 = _mm_max_ps(dx2, min);
			dx3 = _mm_max_ps(dx3, min);
			dx0 = _mm_min_ps(dx0, max);
			dx1 = _mm_min_ps(dx1, max);
			dx2 = _mm_min_ps(dx2, max);
			dx3 = _mm_min_ps(dx3, max);
			dy0 = _mm_cvtps_epi32(dx0);
			dy1 = _mm_cvtps_epi32(dx1);
			dy2 = _mm_cvtps_epi32(dx2);
			dy3 = _mm_cvtps_epi32(dx3);
			dy0 = _mm_packs_epi32(dy0, dy1);
			dy2 = _mm_packs_epi32(dy2, dy3);
			dy0 = _mm_packs_epi16(dy0, dy2);
			_mm_storeu_si128((__m128i *)(pdst + 16 * i), dy0);
		}
	}

	i <<= 4;

	if (i < n) {
		for (; i < n; i++) {
			SAT_S8(pdst[i], psrc[i] * fl);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef	SAT_U8
#define	SAT_U8(out, in)                                              \
	{                                                            \
		mlib_f32 x = (in);                                   \
		x = x > 0.0 ? x : 0.0;                               \
		x = x < 255.0 ? x : 255.0;                           \
		out = x;                                             \
	}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_U8_F32_Sat(
    mlib_u8 *dst,
    const mlib_f32 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_s32 i, count;
	mlib_f32 *psrc = (mlib_f32 *)src;
	mlib_u8 *pdst = (mlib_u8 *)dst;
	__m128 dx0, dx1, dx2, dx3;
	__m128i dy0, dy1, dy2, dy3;
	mlib_f32 fl;
	__m128 flp;
	__m128 max, min;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	PREPARE_CONST(fl, shift + 8);

	flp = _mm_set1_ps(fl);
	max = _mm_set1_ps(255.0);
	min = _mm_set1_ps(0.0);

	if (n < 16 || (mlib_addr)psrc & 15) {
		count = (n < 16) ? n : (((16 - (mlib_addr)psrc) & 15) >> 2);

		for (i = 0; i < count; i++) {
			SAT_U8(pdst[i], psrc[i] * fl);
		}

		n -= count;
		pdst += count;
		psrc += count;
	}

	if (!(15 & (mlib_addr)pdst)) {
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n >> 4; i++) {
			dx0 = _mm_load_ps(psrc + 16 * i);
			dx1 = _mm_load_ps(psrc + 16 * i + 4);
			dx2 = _mm_load_ps(psrc + 16 * i + 8);
			dx3 = _mm_load_ps(psrc + 16 * i + 12);
			dx0 = _mm_mul_ps(dx0, flp);
			dx1 = _mm_mul_ps(dx1, flp);
			dx2 = _mm_mul_ps(dx2, flp);
			dx3 = _mm_mul_ps(dx3, flp);
			dx0 = _mm_max_ps(dx0, min);
			dx1 = _mm_max_ps(dx1, min);
			dx2 = _mm_max_ps(dx2, min);
			dx3 = _mm_max_ps(dx3, min);
			dx0 = _mm_min_ps(dx0, max);
			dx1 = _mm_min_ps(dx1, max);
			dx2 = _mm_min_ps(dx2, max);
			dx3 = _mm_min_ps(dx3, max);
			dy0 = _mm_cvtps_epi32(dx0);
			dy1 = _mm_cvtps_epi32(dx1);
			dy2 = _mm_cvtps_epi32(dx2);
			dy3 = _mm_cvtps_epi32(dx3);
			dy0 = _mm_packs_epi32(dy0, dy1);
			dy2 = _mm_packs_epi32(dy2, dy3);
			dy0 = _mm_packus_epi16(dy0, dy2);
			_mm_store_si128((__m128i *)(pdst + 16 * i), dy0);
		}
	} else {
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n >> 4; i++) {
			dx0 = _mm_load_ps(psrc + 16 * i);
			dx1 = _mm_load_ps(psrc + 16 * i + 4);
			dx2 = _mm_load_ps(psrc + 16 * i + 8);
			dx3 = _mm_load_ps(psrc + 16 * i + 12);
			dx0 = _mm_mul_ps(dx0, flp);
			dx1 = _mm_mul_ps(dx1, flp);
			dx2 = _mm_mul_ps(dx2, flp);
			dx3 = _mm_mul_ps(dx3, flp);
			dx0 = _mm_max_ps(dx0, min);
			dx1 = _mm_max_ps(dx1, min);
			dx2 = _mm_max_ps(dx2, min);
			dx3 = _mm_max_ps(dx3, min);
			dx0 = _mm_min_ps(dx0, max);
			dx1 = _mm_min_ps(dx1, max);
			dx2 = _mm_min_ps(dx2, max);
			dx3 = _mm_min_ps(dx3, max);
			dy0 = _mm_cvtps_epi32(dx0);
			dy1 = _mm_cvtps_epi32(dx1);
			dy2 = _mm_cvtps_epi32(dx2);
			dy3 = _mm_cvtps_epi32(dx3);
			dy0 = _mm_packs_epi32(dy0, dy1);
			dy2 = _mm_packs_epi32(dy2, dy3);
			dy0 = _mm_packus_epi16(dy0, dy2);
			_mm_storeu_si128((__m128i *)(pdst + 16 * i), dy0);
		}
	}

	i <<= 4;

	if (i < n) {
		for (; i < n; i++) {
			SAT_U8(pdst[i], psrc[i] * fl);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef	SAT_S16
#define	SAT_S16(out, in)                                             \
	{                                                            \
		mlib_f32 x = (in);                                   \
		x = x > -32768.0 ? x : -32768.0;                     \
		x = x < 32767.0 ? x : 32767.0;                       \
		out = x;                                             \
	}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_S16_F32_Sat(
    mlib_s16 *dst,
    const mlib_f32 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_s32 i, count;
	mlib_f32 *psrc = (mlib_f32 *)src;
	mlib_s16 *pdst = (mlib_s16 *)dst;
	__m128 dx0, dx1;
	__m128i dy0, dy1;
	mlib_f32 fl;
	__m128 flp;
	__m128 max, min;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	PREPARE_CONST(fl, shift + 15);

	flp = _mm_set1_ps(fl);
	max = _mm_set1_ps(32767.0);
	min = _mm_set1_ps(-32768.0);

	if (n < 16 || (mlib_addr)psrc & 15) {
		count = (n < 16) ? n : (((16 - (mlib_addr)psrc) & 15) >> 2);

		for (i = 0; i < count; i++) {
			SAT_S16(pdst[i], psrc[i] * fl);
		}

		n -= count;
		pdst += count;
		psrc += count;
	}

	if (!(15 & (mlib_addr)pdst)) {
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n >> 3; i++) {
			dx0 = _mm_load_ps(psrc + 8 * i);
			dx1 = _mm_load_ps(psrc + 8 * i + 4);
			dx0 = _mm_mul_ps(dx0, flp);
			dx1 = _mm_mul_ps(dx1, flp);
			dx0 = _mm_max_ps(dx0, min);
			dx1 = _mm_max_ps(dx1, min);
			dx0 = _mm_min_ps(dx0, max);
			dx1 = _mm_min_ps(dx1, max);
			dy0 = _mm_cvtps_epi32(dx0);
			dy1 = _mm_cvtps_epi32(dx1);
			dy0 = _mm_packs_epi32(dy0, dy1);
			_mm_store_si128((__m128i *)(pdst + 8 * i), dy0);
		}
	} else {
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n >> 3; i++) {
			dx0 = _mm_load_ps(psrc + 8 * i);
			dx1 = _mm_load_ps(psrc + 8 * i + 4);
			dx0 = _mm_mul_ps(dx0, flp);
			dx1 = _mm_mul_ps(dx1, flp);
			dx0 = _mm_max_ps(dx0, min);
			dx1 = _mm_max_ps(dx1, min);
			dx0 = _mm_min_ps(dx0, max);
			dx1 = _mm_min_ps(dx1, max);
			dy0 = _mm_cvtps_epi32(dx0);
			dy1 = _mm_cvtps_epi32(dx1);
			dy0 = _mm_packs_epi32(dy0, dy1);
			_mm_storeu_si128((__m128i *)(pdst + 8 * i), dy0);
		}
	}

	i <<= 3;

	if (i < n) {
		for (; i < n; i++) {
			SAT_S16(pdst[i], psrc[i] * fl);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_F32_U8(
    mlib_f32 *dst,
    const mlib_u8 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_s32 i, count;
	mlib_u8 *psrc = (mlib_u8 *)src;
	mlib_f32 *pdst = (mlib_f32 *)dst;
	__m128i dx0, dx1, dx2, dx3;
	__m128 dy0, dy1, dy2, dy3;
	mlib_f32 fl;
	__m128 flp;
	__m128i zero;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	PREPARE_CONST(fl, shift - 8);

	flp = _mm_set1_ps(fl);
	zero = _mm_setzero_si128();

	if (n < 16 || (mlib_addr)pdst & 15) {
		count = (n < 16) ? n : (((16 - (mlib_addr)pdst) & 15) >> 2);

		for (i = 0; i < count; i++) {
			pdst[i] = psrc[i] * fl;
		}

		n -= count;
		pdst += count;
		psrc += count;
	}

	if (!(15 & (mlib_addr)psrc)) {
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n >> 4; i++) {
			dx0 = _mm_load_si128((__m128i *)(psrc + 16 * i));
			dx1 = _mm_unpacklo_epi8(dx0, zero);
			dx3 = _mm_unpackhi_epi8(dx0, zero);
			dx0 = _mm_unpacklo_epi16(dx1, zero);
			dx2 = _mm_unpacklo_epi16(dx3, zero);
			dx1 = _mm_unpackhi_epi16(dx1, zero);
			dx3 = _mm_unpackhi_epi16(dx3, zero);
			dy0 = _mm_cvtepi32_ps(dx0);
			dy1 = _mm_cvtepi32_ps(dx1);
			dy2 = _mm_cvtepi32_ps(dx2);
			dy3 = _mm_cvtepi32_ps(dx3);
			dy0 = _mm_mul_ps(dy0, flp);
			dy1 = _mm_mul_ps(dy1, flp);
			dy2 = _mm_mul_ps(dy2, flp);
			dy3 = _mm_mul_ps(dy3, flp);
			_mm_store_ps(pdst + 16 * i, dy0);
			_mm_store_ps(pdst + 16 * i + 4, dy1);
			_mm_store_ps(pdst + 16 * i + 8, dy2);
			_mm_store_ps(pdst + 16 * i + 12, dy3);
		}
	} else {
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n >> 4; i++) {
			dx0 = _mm_loadu_si128((__m128i *)(psrc + 16 * i));
			dx1 = _mm_unpacklo_epi8(dx0, zero);
			dx3 = _mm_unpackhi_epi8(dx0, zero);
			dx0 = _mm_unpacklo_epi16(dx1, zero);
			dx2 = _mm_unpacklo_epi16(dx3, zero);
			dx1 = _mm_unpackhi_epi16(dx1, zero);
			dx3 = _mm_unpackhi_epi16(dx3, zero);
			dy0 = _mm_cvtepi32_ps(dx0);
			dy1 = _mm_cvtepi32_ps(dx1);
			dy2 = _mm_cvtepi32_ps(dx2);
			dy3 = _mm_cvtepi32_ps(dx3);
			dy0 = _mm_mul_ps(dy0, flp);
			dy1 = _mm_mul_ps(dy1, flp);
			dy2 = _mm_mul_ps(dy2, flp);
			dy3 = _mm_mul_ps(dy3, flp);
			_mm_store_ps(pdst + 16 * i, dy0);
			_mm_store_ps(pdst + 16 * i + 4, dy1);
			_mm_store_ps(pdst + 16 * i + 8, dy2);
			_mm_store_ps(pdst + 16 * i + 12, dy3);
		}
	}

	i <<= 4;

	if (i < n) {
		for (; i < n; i++) {
			pdst[i] = psrc[i] * fl;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_F32_S8(
    mlib_f32 *dst,
    const mlib_s8 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_s32 i, count;
	mlib_s8 *psrc = (mlib_s8 *)src;
	mlib_f32 *pdst = (mlib_f32 *)dst;
	__m128i dx0, dx1, dx2, dx3;
	__m128 dy0, dy1, dy2, dy3;
	mlib_f32 fl0, fl;
	__m128 flp;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	PREPARE_CONST(fl0, shift - 7);
	fl = fl0 * (1.0f / (1 << 24));

	flp = _mm_set1_ps(fl);

	if (n < 16 || (mlib_addr)pdst & 15) {
		count = (n < 16) ? n : (((16 - (mlib_addr)pdst) & 15) >> 2);

		for (i = 0; i < count; i++) {
			pdst[i] = psrc[i] * fl0;
		}

		n -= count;
		pdst += count;
		psrc += count;
	}

	if (!(15 & (mlib_addr)psrc)) {
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n >> 4; i++) {
			dx0 = _mm_load_si128((__m128i *)(psrc + 16 * i));
			dx1 = _mm_unpacklo_epi8(dx0, dx0);
			dx3 = _mm_unpackhi_epi8(dx0, dx0);
			dx0 = _mm_unpacklo_epi16(dx1, dx1);
			dx2 = _mm_unpacklo_epi16(dx3, dx3);
			dx1 = _mm_unpackhi_epi16(dx1, dx1);
			dx3 = _mm_unpackhi_epi16(dx3, dx3);
			dx0 = _mm_slli_epi32(dx0, 24);
			dx1 = _mm_slli_epi32(dx1, 24);
			dx2 = _mm_slli_epi32(dx2, 24);
			dx3 = _mm_slli_epi32(dx3, 24);
			dy0 = _mm_cvtepi32_ps(dx0);
			dy1 = _mm_cvtepi32_ps(dx1);
			dy2 = _mm_cvtepi32_ps(dx2);
			dy3 = _mm_cvtepi32_ps(dx3);
			dy0 = _mm_mul_ps(dy0, flp);
			dy1 = _mm_mul_ps(dy1, flp);
			dy2 = _mm_mul_ps(dy2, flp);
			dy3 = _mm_mul_ps(dy3, flp);
			_mm_store_ps(pdst + 16 * i, dy0);
			_mm_store_ps(pdst + 16 * i + 4, dy1);
			_mm_store_ps(pdst + 16 * i + 8, dy2);
			_mm_store_ps(pdst + 16 * i + 12, dy3);
		}
	} else {
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n >> 4; i++) {
			dx0 = _mm_loadu_si128((__m128i *)(psrc + 16 * i));
			dx1 = _mm_unpacklo_epi8(dx0, dx0);
			dx3 = _mm_unpackhi_epi8(dx0, dx0);
			dx0 = _mm_unpacklo_epi16(dx1, dx1);
			dx2 = _mm_unpacklo_epi16(dx3, dx3);
			dx1 = _mm_unpackhi_epi16(dx1, dx1);
			dx3 = _mm_unpackhi_epi16(dx3, dx3);
			dx0 = _mm_slli_epi32(dx0, 24);
			dx1 = _mm_slli_epi32(dx1, 24);
			dx2 = _mm_slli_epi32(dx2, 24);
			dx3 = _mm_slli_epi32(dx3, 24);
			dy0 = _mm_cvtepi32_ps(dx0);
			dy1 = _mm_cvtepi32_ps(dx1);
			dy2 = _mm_cvtepi32_ps(dx2);
			dy3 = _mm_cvtepi32_ps(dx3);
			dy0 = _mm_mul_ps(dy0, flp);
			dy1 = _mm_mul_ps(dy1, flp);
			dy2 = _mm_mul_ps(dy2, flp);
			dy3 = _mm_mul_ps(dy3, flp);
			_mm_store_ps(pdst + 16 * i, dy0);
			_mm_store_ps(pdst + 16 * i + 4, dy1);
			_mm_store_ps(pdst + 16 * i + 8, dy2);
			_mm_store_ps(pdst + 16 * i + 12, dy3);
		}
	}

	i <<= 4;

	if (i < n) {
		for (; i < n; i++) {
			pdst[i] = psrc[i] * fl0;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_F32_S16(
    mlib_f32 *dst,
    const mlib_s16 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_s32 i, count;
	mlib_s16 *psrc = (mlib_s16 *)src;
	mlib_f32 *pdst = (mlib_f32 *)dst;
	__m128i dx0, dx1;
	__m128 dy0, dy1;
	mlib_f32 fl0, fl;
	__m128 flp;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	PREPARE_CONST(fl0, shift - 15);
	fl = fl0 * (1.0f / (1 << 16));

	flp = _mm_set1_ps(fl);

	if (n < 16 || (mlib_addr)pdst & 15) {
		count = (n < 16) ? n : (((16 - (mlib_addr)pdst) & 15) >> 2);

		for (i = 0; i < count; i++) {
			pdst[i] = psrc[i] * fl0;
		}

		n -= count;
		pdst += count;
		psrc += count;
	}

	if (!(15 & (mlib_addr)psrc)) {
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n >> 3; i++) {
			dx1 = _mm_load_si128((__m128i *)(psrc + 8 * i));
			dx0 = _mm_unpacklo_epi16(dx1, dx1);
			dx1 = _mm_unpackhi_epi16(dx1, dx1);
			dx0 = _mm_slli_epi32(dx0, 16);
			dx1 = _mm_slli_epi32(dx1, 16);
			dy0 = _mm_cvtepi32_ps(dx0);
			dy1 = _mm_cvtepi32_ps(dx1);
			dy0 = _mm_mul_ps(dy0, flp);
			dy1 = _mm_mul_ps(dy1, flp);
			_mm_store_ps(pdst + 8 * i, dy0);
			_mm_store_ps(pdst + 8 * i + 4, dy1);
		}
	} else {
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n >> 3; i++) {
			dx1 = _mm_loadu_si128((__m128i *)(psrc + 8 * i));
			dx0 = _mm_unpacklo_epi16(dx1, dx1);
			dx1 = _mm_unpackhi_epi16(dx1, dx1);
			dx0 = _mm_slli_epi32(dx0, 16);
			dx1 = _mm_slli_epi32(dx1, 16);
			dy0 = _mm_cvtepi32_ps(dx0);
			dy1 = _mm_cvtepi32_ps(dx1);
			dy0 = _mm_mul_ps(dy0, flp);
			dy1 = _mm_mul_ps(dy1, flp);
			_mm_store_ps(pdst + 8 * i, dy0);
			_mm_store_ps(pdst + 8 * i + 4, dy1);
		}
	}

	i <<= 3;

	if (i < n) {
		for (; i < n; i++) {
			pdst[i] = psrc[i] * fl0;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_U8S_F32S_Sat(
    mlib_u8 *dst,
    const mlib_f32 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	return (__mlib_SignalConvertShift_U8_F32_Sat(dst, src, shift, n << 1));
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_S8S_F32S_Sat(
    mlib_s8 *dst,
    const mlib_f32 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	return (__mlib_SignalConvertShift_S8_F32_Sat(dst, src, shift, n << 1));
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_S16S_F32S_Sat(
    mlib_s16 *dst,
    const mlib_f32 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	return (__mlib_SignalConvertShift_S16_F32_Sat(dst, src, shift, n << 1));
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_F32S_U8S(
    mlib_f32 *dst,
    const mlib_u8 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	return (__mlib_SignalConvertShift_F32_U8(dst, src, shift, n << 1));
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_F32S_S8S(
    mlib_f32 *dst,
    const mlib_s8 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	return (__mlib_SignalConvertShift_F32_S8(dst, src, shift, n << 1));
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_F32S_S16S(
    mlib_f32 *dst,
    const mlib_s16 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	return (__mlib_SignalConvertShift_F32_S16(dst, src, shift, n << 1));
}

/* *********************************************************** */

#undef	SAT_S32
#define	SAT_S32(out, in)                                             \
	{                                                            \
		mlib_d64 x = (in);                                   \
		x = x > -2147483648.0 ? x : -2147483648.0;           \
		x = x < 2147483647.0 ? x : 2147483647.0;             \
		out = (mlib_s32)x;                                   \
	}

/* *********************************************************** */

typedef	union
{
	__m128 f;
	__m128i i;
} u128;

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_S32_F32_Sat(
    mlib_s32 *dst,
    const mlib_f32 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_s32 i, count;
	mlib_f32 *psrc = (mlib_f32 *)src;
	mlib_s32 *pdst = (mlib_s32 *)dst;
	__m128 dx0;
	__m128i dy0, dy1;
	mlib_f32 fl0;
	__m128 flp, max, min;
	u128 mask;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	PREPARE_CONST(fl0, shift + 31);

	flp = _mm_set1_ps(fl0);
	/* can not be 2147483647.0 because of the FP precision problem */
	max = _mm_set1_ps(2147483520.0);
	min = _mm_set1_ps(-2147483648.0);

	if (n < 16 || (mlib_addr)psrc & 15) {
		count = (n < 16) ? n : (((16 - (mlib_addr)psrc) & 15) >> 2);

		for (i = 0; i < count; i++) {
			SAT_S32(pdst[i], psrc[i] * fl0);
		}

		n -= count;
		pdst += count;
		psrc += count;
	}

	if (!(15 & (mlib_addr)pdst)) {
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n >> 2; i++) {
			dx0 = _mm_load_ps(psrc + 4 * i);
			dx0 = _mm_mul_ps(dx0, flp);
			mask.f = _mm_cmpgt_ps(dx0, max);
			dy1 = _mm_srli_epi32(mask.i, 1);
			dx0 = _mm_max_ps(dx0, min);
			dy0 = _mm_cvtps_epi32(dx0);
			dy0 = _mm_andnot_si128(mask.i, dy0);
			dy0 = _mm_or_si128(dy0, dy1);
			_mm_store_si128((__m128i *)(pdst + 4 * i), dy0);
		}
	} else {
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n >> 2; i++) {
			dx0 = _mm_load_ps(psrc + 4 * i);
			dx0 = _mm_mul_ps(dx0, flp);
			mask.f = _mm_cmpgt_ps(dx0, max);
			dy1 = _mm_srli_epi32(mask.i, 1);
			dx0 = _mm_max_ps(dx0, min);
			dy0 = _mm_cvtps_epi32(dx0);
			dy0 = _mm_andnot_si128(mask.i, dy0);
			dy0 = _mm_or_si128(dy0, dy1);
			_mm_storeu_si128((__m128i *)(pdst + 4 * i), dy0);
		}
	}

	i <<= 2;

	if (i < n) {
		for (; i < n; i++) {
			SAT_S32(pdst[i], psrc[i] * fl0);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_F32_S32(
    mlib_f32 *dst,
    const mlib_s32 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_s32 i, count;
	mlib_s32 *psrc = (mlib_s32 *)src;
	mlib_f32 *pdst = (mlib_f32 *)dst;
	__m128i dx0;
	__m128 dy0;
	mlib_f32 fl0;
	__m128 flp;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	PREPARE_CONST(fl0, shift - 31);

	flp = _mm_set1_ps(fl0);

	if (n < 16 || (mlib_addr)psrc & 15) {
		count = (n < 16) ? n : (((16 - (mlib_addr)psrc) & 15) >> 2);

		for (i = 0; i < count; i++) {
			pdst[i] = psrc[i] * fl0;
		}

		n -= count;
		pdst += count;
		psrc += count;
	}

	if (!(15 & (mlib_addr)pdst)) {
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n >> 2; i++) {
			dx0 = _mm_load_si128((__m128i *)(psrc + 4 * i));
			dy0 = _mm_cvtepi32_ps(dx0);
			dy0 = _mm_mul_ps(dy0, flp);
			_mm_store_ps(pdst + 4 * i, dy0);
		}
	} else {
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n >> 2; i++) {
			dx0 = _mm_load_si128((__m128i *)(psrc + 4 * i));
			dy0 = _mm_cvtepi32_ps(dx0);
			dy0 = _mm_mul_ps(dy0, flp);
			_mm_storeu_ps(pdst + 4 * i, dy0);
		}
	}

	i <<= 2;

	if (i < n) {
		for (; i < n; i++) {
			pdst[i] = psrc[i] * fl0;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_S32S_F32S_Sat(
    mlib_s32 *dst,
    const mlib_f32 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	return (__mlib_SignalConvertShift_S32_F32_Sat(dst, src, shift, n << 1));
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_F32S_S32S(
    mlib_f32 *dst,
    const mlib_s32 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	return (__mlib_SignalConvertShift_F32_S32(dst, src, shift, n << 1));
}

/* *********************************************************** */
