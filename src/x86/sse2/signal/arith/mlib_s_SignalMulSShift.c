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

#pragma ident	"@(#)mlib_s_SignalMulSShift.c	9.4	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalMulSShift_[S16|S16S]_Sat,
 *      mlib_SignalMulSShift_[S16|S16S]_[S16|S16S]_Sat - multiplication of
 *                     mlib_s32 16-bit format signal array by a scalar
 *                     with shifting
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalMulSShift_S16S_S16S_Sat(mlib_s16       *dst,
 *                                                     const mlib_s16 *src,
 *                                                     const mlib_s16 *c,
 *                                                     mlib_s32       shift,
 *                                                     mlib_s32       n);
 *      mlib_status mlib_SignalMulSShift_S16S_Sat(mlib_s16       *data,
 *                                                const mlib_s16 *c,
 *                                                mlib_s32       shift,
 *                                                mlib_s32       n);
 *      mlib_status mlib_SignalMulSShift_S16_S16_Sat(mlib_s16       *dst,
 *                                                   const mlib_s16 *src,
 *                                                   const mlib_s16 *c,
 *                                                   mlib_s32       shift,
 *                                                   mlib_s32       n);
 *      mlib_status mlib_SignalMulSShift_S16_Sat(mlib_s16       *data,
 *                                               const mlib_s16 *c,
 *                                               mlib_s32       shift,
 *                                               mlib_s32       n);
 *
 * ARGUMENTS
 *      data  Input and output signal array
 *      dst   Output signal array
 *      src   Input signal array
 *      c     Scaling factor. The scaling factor is in Q15 format
 *      shift Left shifting factor, may be negative
 *      n     Number of samples in the input signal arrays.
 *
 * DESCRIPTION
 *      dst = src * c * (2 ** shift)
 */

#include <mlib_signal.h>
#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif


/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalMulSShift_S16S_S16S_Sat = \
	__mlib_SignalMulSShift_S16S_S16S_Sat
#pragma weak mlib_SignalMulSShift_S16_S16_Sat = \
	__mlib_SignalMulSShift_S16_S16_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalMulSShift_S16S_S16S_Sat)
    mlib_SignalMulSShift_S16S_S16S_Sat
    __attribute__((weak, alias("__mlib_SignalMulSShift_S16S_S16S_Sat")));
__typeof__(__mlib_SignalMulSShift_S16_S16_Sat) mlib_SignalMulSShift_S16_S16_Sat
    __attribute__((weak, alias("__mlib_SignalMulSShift_S16_S16_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */
#define	SAT_S32_UPPER(x)  \
{ \
	mask0 = _mm_sub_epi64(dupper_max, x);       \
	imm1 = _mm_extract_epi16(mask0, 7);         \
	if (imm1 < (mlib_s32) 0x8000) {             \
		mask0 = _mm_sub_epi64(x, dupper_min);   \
		imm2 = _mm_extract_epi16(mask0, 7);     \
		if (imm2 != 0) {                        \
			x = _mm_and_si128(x, dzero_f);      \
			x = _mm_or_si128(x, dupper_min);    \
		}                                       \
	} else {                                    \
		x = _mm_and_si128(x, dzero_f);          \
		x = _mm_or_si128(x, dupper_max);        \
	}\
}


/* *********************************************************** */
#define	SAT_S32_LOWER(x)  \
{                                               \
	mask0 = _mm_sub_epi64(dlower_max, x);       \
	imm1 = _mm_extract_epi16(mask0, 3);         \
	if (imm1 < (mlib_s32) 0x8000) {             \
		mask0 = _mm_sub_epi64(x, dlower_min);   \
		imm2 = _mm_extract_epi16(mask0, 3);     \
		if (imm2 != 0) {                        \
			x = _mm_and_si128(x, df_zero);      \
			(x) = _mm_or_si128(x, dlower_min);  \
		}                                       \
	} else {                                    \
		x = _mm_and_si128((x), df_zero);        \
		(x) = _mm_or_si128((x), dlower_max);    \
	}                                           \
}

/* *********************************************************** */
#define	FLOAT2INT_CLAMP(DST, SRC)           \
{                                                       \
	mlib_d64 dsrc = (mlib_d64)(SRC);                    \
	\
	if (dsrc > (mlib_d64)MLIB_S32_MAX)                  \
	dsrc = (mlib_d64)MLIB_S32_MAX;                  \
	if (dsrc < (mlib_d64)MLIB_S32_MIN)                  \
	dsrc = (mlib_d64)MLIB_S32_MIN;                  \
	DST = (mlib_s32)dsrc >> 16;                         \
}

#define	LOAD_X                        \
{ \
		if (0 == ((mlib_addr)src & 0xf)) { \
			dx = _mm_load_si128(dpx);       \
		} else {        \
			dx = _mm_loadu_si128(dpx);   \
		}                           \
}

/* *********************************************************** */
#define	STORE_Z     \
		if (0 == ((mlib_addr)dst & 0xf)) { \
			_mm_store_si128(dpz, dz); \
		} else {  \
			_mm_storeu_si128(dpz, dz);  \
		}

/* *********************************************************** */
#define	MUL_2S16_2S32 \
	dlower_upper = _mm_mulhi_epi16(dx, dy);         \
	dlower_lower = _mm_mullo_epi16(dx, dy);         \
	dlower = _mm_unpacklo_epi16(dlower_lower, dlower_upper);\
	dupper = _mm_unpackhi_epi16(dlower_lower, dlower_upper);

/* *********************************************************** */

mlib_status
__mlib_SignalMulSShift_S16_S16_Sat(
    mlib_s16 *dst,
    const mlib_s16 *src,
    const mlib_s16 *c,
    mlib_s32 shift,
    mlib_s32 n)
{
	__m128i *dpx, *dpz;
	__m128i dx, dy, dz;
	__m128i dupper, dlower;
	__m128i dupper_lower, dupper_upper;
	__m128i dlower_upper, dlower_lower;
	__m128i dsign;
	mlib_s32 ncurrent, nlen;
	mlib_s32 i;
	mlib_s32 sh1 = 0;
	dpx = (__m128i *) src;
	dpz = (__m128i *) dst;
	nlen = n >> 3;

	dy = _mm_set1_epi16(c[0]);

	if (shift <= -1) {
		if (shift < -16) {
			shift = -16;
		}
		sh1 = -shift + 15;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (i = 0; i < nlen; i++) {
			LOAD_X;
			MUL_2S16_2S32;
			dlower = _mm_srai_epi32(dlower, sh1);
			dupper = _mm_srai_epi32(dupper, sh1);
			dz = _mm_packs_epi32(dlower, dupper);
			STORE_Z;
			dpx++;
			dpz++;
		}
	} else {
		if (shift > 31) {
			shift = 31;
		}
		sh1 = shift + 1;
		__m128i mask0;
		__m128i df_zero = _mm_set_epi32(0xffffffff,
				0xffffffff, 0, 0);
		__m128i dzero_f = _mm_set_epi32(0, 0,
				0xffffffff, 0xffffffff);
		__m128i dupper_max = _mm_set_epi32(0,
				MLIB_S32_MAX, 0, 0);
		__m128i dupper_min = _mm_set_epi32(
				0xffffffff, MLIB_S32_MIN, 0, 0);
		__m128i dlower_min = _mm_set_epi32(0, 0,
				0xffffffff, MLIB_S32_MIN);
		__m128i dlower_max = _mm_set_epi32(0, 0,
				0, MLIB_S32_MAX);
		mlib_s32 imm1, imm2;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (i = 0; i < nlen; i++) {
			LOAD_X;
			MUL_2S16_2S32;
			dsign = _mm_srai_epi32(dlower, 31);
			dlower_lower = _mm_unpacklo_epi32(dlower, dsign);
			dlower_upper = _mm_unpackhi_epi32(dlower, dsign);
			dsign = _mm_srai_epi32(dupper, 31);
			dupper_lower = _mm_unpacklo_epi32(dupper, dsign);
			dupper_upper = _mm_unpackhi_epi32(dupper, dsign);
			dlower_lower = _mm_slli_epi64(dlower_lower, sh1);
			dlower_upper = _mm_slli_epi64(dlower_upper, sh1);
			dupper_lower = _mm_slli_epi64(dupper_lower, sh1);
			dupper_upper = _mm_slli_epi64(dupper_upper, sh1);
/* Saturation */
			SAT_S32_UPPER(dlower_lower);
			SAT_S32_LOWER(dlower_lower);
			SAT_S32_UPPER(dlower_upper);
			SAT_S32_LOWER(dlower_upper);
			SAT_S32_UPPER(dupper_lower);
			SAT_S32_LOWER(dupper_lower);
			SAT_S32_UPPER(dupper_upper);
			SAT_S32_LOWER(dupper_upper);
/* merge all data */
			dlower_lower = _mm_srai_epi32(dlower_lower, 16);
			dlower_upper = _mm_srai_epi32(dlower_upper, 16);
			dupper_lower = _mm_srai_epi32(dupper_lower, 16);
			dupper_upper = _mm_srai_epi32(dupper_upper, 16);
			dlower_lower = _mm_packs_epi32(dlower_lower,
						dlower_upper);
			dupper_upper = _mm_packs_epi32(dupper_lower,
						dupper_upper);

			dz = _mm_packs_epi32(dlower_lower, dupper_upper);
			STORE_Z;
			dpx++;
			dpz++;
		}
	}
	if ((nlen << 3) != n) {
		ncurrent = nlen << 3;
		mlib_d64 d_val;
		if (shift >= 0) {
			d_val = 2. * ((mlib_u32)1 << (shift));
		} else {
			d_val = 2. / ((mlib_u32)1 << (-shift));
		}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (i = ncurrent; i < n; i++) {
			FLOAT2INT_CLAMP(dst[i], d_val * src[i] * c[0]);
		}
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulSShift_S16S_S16S_Sat(
    mlib_s16 *dst,
    const mlib_s16 *src,
    const mlib_s16 *c,
    mlib_s32 shift,
    mlib_s32 n)
{
	__m128i *dpx, *dpz;
	__m128i dx, dy, dz;
	__m128i dupper, dlower;
	__m128i dupper_lower, dupper_upper;
	__m128i dlower_upper, dlower_lower;
	__m128i dsign;
	mlib_s32 ncurrent, nlen;
	mlib_s32 i;
	mlib_s32 sh1 = 0;
	dpx = (__m128i *) src;
	dpz = (__m128i *) dst;
	nlen = n >> 2;

	dy = _mm_set_epi16(c[1], c[0], c[1], c[0],
			c[1], c[0], c[1], c[0]);

	if (shift <= -1) {
		if (shift < -16) {
			shift = -16;
		}
		sh1 = -shift + 15;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (i = 0; i < nlen; i++) {
			LOAD_X;
			MUL_2S16_2S32;
			dlower = _mm_srai_epi32(dlower, sh1);
			dupper = _mm_srai_epi32(dupper, sh1);
			dz = _mm_packs_epi32(dlower, dupper);
			STORE_Z;
			dpx++;
			dpz++;
		}
	} else {
		if (shift > 31) {
			shift = 31;
		}
		sh1 = shift + 1;
		__m128i mask0;
		__m128i df_zero = _mm_set_epi32(0xffffffff,
				0xffffffff, 0, 0);
		__m128i dzero_f = _mm_set_epi32(0, 0,
				0xffffffff, 0xffffffff);
		__m128i dupper_max = _mm_set_epi32(0,
				MLIB_S32_MAX, 0, 0);
		__m128i dupper_min = _mm_set_epi32(
				0xffffffff, MLIB_S32_MIN, 0, 0);
		__m128i dlower_min = _mm_set_epi32(0, 0,
				0xffffffff, MLIB_S32_MIN);
		__m128i dlower_max = _mm_set_epi32(0, 0,
				0, MLIB_S32_MAX);
		mlib_s32 imm1, imm2;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (i = 0; i < nlen; i++) {
			LOAD_X;
			MUL_2S16_2S32;
			dsign = _mm_srai_epi32(dlower, 31);
			dlower_lower = _mm_unpacklo_epi32(dlower, dsign);
			dlower_upper = _mm_unpackhi_epi32(dlower, dsign);
			dsign = _mm_srai_epi32(dupper, 31);
			dupper_lower = _mm_unpacklo_epi32(dupper, dsign);
			dupper_upper = _mm_unpackhi_epi32(dupper, dsign);
			dlower_lower = _mm_slli_epi64(dlower_lower, sh1);
			dlower_upper = _mm_slli_epi64(dlower_upper, sh1);
			dupper_lower = _mm_slli_epi64(dupper_lower, sh1);
			dupper_upper = _mm_slli_epi64(dupper_upper, sh1);
/* Saturation */
			SAT_S32_UPPER(dlower_lower);
			SAT_S32_LOWER(dlower_lower);
			SAT_S32_UPPER(dlower_upper);
			SAT_S32_LOWER(dlower_upper);
			SAT_S32_UPPER(dupper_lower);
			SAT_S32_LOWER(dupper_lower);
			SAT_S32_UPPER(dupper_upper);
			SAT_S32_LOWER(dupper_upper);
/* merge all data */
			dlower_lower = _mm_srai_epi32(dlower_lower, 16);
			dlower_upper = _mm_srai_epi32(dlower_upper, 16);
			dupper_lower = _mm_srai_epi32(dupper_lower, 16);
			dupper_upper = _mm_srai_epi32(dupper_upper, 16);
			dlower_lower = _mm_packs_epi32(dlower_lower,
						dlower_upper);
			dupper_upper = _mm_packs_epi32(dupper_lower,
						dupper_upper);

			dz = _mm_packs_epi32(dlower_lower, dupper_upper);
			STORE_Z;
			dpx++;
			dpz++;
		}
	}
	if ((nlen << 2) != n) {
		ncurrent = nlen << 2;
		mlib_d64 d_val;
		if (shift >= 0) {
			d_val = 2. * ((mlib_u32)1 << (shift));
		} else {
			d_val = 2. / ((mlib_u32)1 << (-shift));
		}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (i = ncurrent; i < n; i++) {
			FLOAT2INT_CLAMP(dst[2 * i], d_val * src[2 * i] * c[0]);
			FLOAT2INT_CLAMP(dst[2 * i + 1], d_val *
					src[2 * i + 1] * c[1]);
		}
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */
