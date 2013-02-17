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

#pragma ident	"@(#)mlib_s_SignalUpSampleFIR_f.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      Internal functions for mlib_SignalUpSampleFIR_*
 *
 */

#include <mlib_signal.h>
#include <mlib_SignalUpSampleFIRFunc.h>
#include <mlib_Utils.h>
#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#define	CLAMP_F32(DST, SRC)	DST = (mlib_f32)(SRC)

/* *********************************************************** */

void
mlib_SignalFIR_s16_to_d64(
    mlib_d64 *pdst,
    mlib_s16 *psrc,
    mlib_s32 n)
{
	mlib_s32 i;

	for (i = 0; (i < n) && ((mlib_addr)psrc & 15); i++) {
		(*pdst++) = (mlib_d64)(*psrc++);
	}

	if (((mlib_addr)pdst & 15) == 0) {

	for (; i < n - 7; i += 8) {
		__m128i x_sd0, x_sd1, x_sd2, x_sd3, x_sd4;
		__m128d xd_sd0, xd_sd1, xd_sd2, xd_sd3;
		x_sd0 = _mm_load_si128((__m128i *)psrc);
		psrc += 8;
		x_sd1 = _mm_srai_epi16(x_sd0, 15);
		x_sd2 = _mm_unpacklo_epi16(x_sd0, x_sd1);
		x_sd3 = _mm_unpackhi_epi16(x_sd0, x_sd1);
		xd_sd0 = _mm_cvtepi32_pd(x_sd2);
		xd_sd1 = _mm_cvtepi32_pd(_mm_srli_si128(x_sd2, 8));
		xd_sd2 = _mm_cvtepi32_pd(x_sd3);
		xd_sd3 = _mm_cvtepi32_pd(_mm_srli_si128(x_sd3, 8));
		_mm_store_pd(pdst, xd_sd0);
		_mm_store_pd((pdst + 2), xd_sd1);
		_mm_store_pd((pdst + 4), xd_sd2);
		_mm_store_pd((pdst + 6), xd_sd3);
		pdst += 8;
	}

	if (i < n - 3) {
		__m128i x_sd0, x_sd1, x_sd2, x_sd3, x_sd4;
		__m128d xd_sd0, xd_sd1;
		x_sd0 = _mm_loadl_epi64((void *)psrc);
		psrc += 4;
		x_sd1 = _mm_srai_epi16(x_sd0, 15);
		x_sd2 = _mm_unpacklo_epi16(x_sd0, x_sd1);
		xd_sd0 = _mm_cvtepi32_pd(x_sd2);
		xd_sd1 = _mm_cvtepi32_pd(_mm_srli_si128(x_sd2, 8));
		_mm_store_pd(pdst, xd_sd0);
		_mm_store_pd((pdst + 2), xd_sd1);
		pdst += 4;
		i += 4;
	}

	} else {

	for (; i < n - 7; i += 8) {
		__m128i x_sd0, x_sd1, x_sd2, x_sd3, x_sd4;
		__m128d xd_sd0, xd_sd1, xd_sd2, xd_sd3;
		x_sd0 = _mm_load_si128((__m128i *)psrc);
		x_sd1 = _mm_srai_epi16(x_sd0, 15);
		x_sd2 = _mm_unpacklo_epi16(x_sd0, x_sd1);
		x_sd3 = _mm_unpackhi_epi16(x_sd0, x_sd1);
		xd_sd0 = _mm_cvtepi32_pd(x_sd2);
		xd_sd1 = _mm_cvtepi32_pd(_mm_srli_si128(x_sd2, 8));
		xd_sd2 = _mm_cvtepi32_pd(x_sd3);
		xd_sd3 = _mm_cvtepi32_pd(_mm_srli_si128(x_sd3, 8));
		_mm_storeu_pd(pdst, xd_sd0);
		_mm_storeu_pd((pdst + 2), xd_sd1);
		_mm_storeu_pd((pdst + 4), xd_sd2);
		_mm_storeu_pd((pdst + 6), xd_sd3);
		psrc += 8;
		pdst += 8;
	}

	if (i < n - 3) {
		__m128i x_sd0, x_sd1, x_sd2, x_sd3, x_sd4;
		__m128d xd_sd0, xd_sd1;
		x_sd0 = _mm_loadl_epi64((void *)psrc);
		psrc += 4;
		x_sd1 = _mm_srai_epi16(x_sd0, 15);
		x_sd2 = _mm_unpacklo_epi16(x_sd0, x_sd1);
		xd_sd0 = _mm_cvtepi32_pd(x_sd2);
		xd_sd1 = _mm_cvtepi32_pd(_mm_srli_si128(x_sd2, 8));
		_mm_storeu_pd(pdst, xd_sd0);
		_mm_storeu_pd((pdst + 2), xd_sd1);
		pdst += 4;
		i += 4;
	}
	}

	for (; i < n; i++) {
		(*pdst++) = (mlib_d64)(*psrc++);
	}
}

void
mlib_SignalFIR_f32_to_d64(
    mlib_d64 *pdst,
    mlib_f32 *psrc,
    mlib_s32 n)
{
	mlib_s32 i;

	for (i = 0; i < n; i++) {
		pdst[i] = psrc[i];
	}
}

/* *********************************************************** */

void
mlib_SignalFIR_d64_to_s16(
    mlib_s16 *pdst,
    mlib_d64 *psrc,
    mlib_s32 n)
{
	mlib_s32 i;

#ifndef MLIB_USE_FTOI_CLAMPING
	for (i = 0; i < n; i++) {
		CLAMP_S16(pdst[i], psrc[i]);
	}

#else /* MLIB_USE_FTOI_CLAMPING */

#ifdef _NO_LONGLONG
	mlib_u32 data;

	if (((mlib_addr)pdst & 3) != 0) {
		(*pdst++) = ((mlib_s32)(*psrc++)) >> 16;
		n--;
	}

	for (i = 0; i < n - 1; i += 2) {
		data = ((mlib_u32)(mlib_s32)psrc[0]) & ~0xFFFF;
		data += (((mlib_u32)(mlib_s32)psrc[1]) >> 16);

		*(mlib_u32 *)pdst = data;
		psrc += 2;
		pdst += 2;
	}

	if (i < n)
		*pdst = ((mlib_s32)*psrc) >> 16;

#else /* _NO_LONGLONG */

	mlib_u64 data;

	for (i = 0; i < n && ((mlib_addr)pdst & 7) != 0; i++) {
		(*pdst++) = ((mlib_s32)(*psrc++)) >> 16;
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < n - 3; i += 4) {
		data = ((mlib_u32)(mlib_s32)psrc[0]) & ~0xFFFF;
		data += (((mlib_u32)(mlib_s32)psrc[1]) >> 16);
		data <<= 16;
		data += (((mlib_u32)(mlib_s32)psrc[2]) >> 16);
		data <<= 16;
		data += (((mlib_u32)(mlib_s32)psrc[3]) >> 16);

		*(mlib_u64 *)pdst = data;
		psrc += 4;
		pdst += 4;
	}

	for (; i < n; i++) {
		(*pdst++) = ((mlib_s32)(*psrc++)) >> 16;
	}

#endif /* _NO_LONGLONG */
#endif /* MLIB_USE_FTOI_CLAMPING */
}

/* *********************************************************** */

void
mlib_SignalFIR_d64_to_f32(
    mlib_f32 *pdst,
    mlib_d64 *psrc,
    mlib_s32 n)
{
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		pdst[i] = psrc[i];
	}
}

/* *********************************************************** */

void
mlib_SignalFIR_s16_to_f32(
    mlib_f32 *pdst,
    mlib_s16 *psrc,
    mlib_s32 n)
{
	mlib_s32 i;

#ifdef _NO_LONGLONG
	mlib_u32 data;

	if (((mlib_addr)psrc & 3) != 0) {
		(*pdst++) = (mlib_f32)(*psrc++);
		n--;
	}

	for (i = 0; i < n - 1; i += 2) {
		data = *(mlib_u32 *)psrc;

#ifdef _LITTLE_ENDIAN
		pdst[1] = (mlib_f32)(mlib_s16)(data >> 16);
		pdst[0] = (mlib_f32)(mlib_s16)(data);
#else /* _LITTLE_ENDIAN */
		pdst[0] = (mlib_f32)(mlib_s16)(data >> 16);
		pdst[1] = (mlib_f32)(mlib_s16)(data);
#endif /* _LITTLE_ENDIAN */

		psrc += 2;
		pdst += 2;
	}

	if (i < n)
		*pdst = (mlib_f32)*psrc;

#else /* _NO_LONGLONG */
	mlib_u64 data;

	for (i = 0; i < n && (7 & (mlib_addr)psrc) != 0; i++)
		(*pdst++) = (mlib_f32)(*psrc++);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < n - 3; i += 4) {
		data = *(mlib_u64 *)psrc;

#ifdef _LITTLE_ENDIAN
		pdst[3] = (mlib_f32)(mlib_s16)(data >> 48);
		pdst[2] = (mlib_f32)(mlib_s16)(data >> 32);
		pdst[1] = (mlib_f32)(mlib_s16)(data >> 16);
		pdst[0] = (mlib_f32)(mlib_s16)(data);
#else /* _LITTLE_ENDIAN */
		pdst[0] = (mlib_f32)(mlib_s16)(data >> 48);
		pdst[1] = (mlib_f32)(mlib_s16)(data >> 32);
		pdst[2] = (mlib_f32)(mlib_s16)(data >> 16);
		pdst[3] = (mlib_f32)(mlib_s16)(data);
#endif /* _LITTLE_ENDIAN */

		psrc += 4;
		pdst += 4;
	}

	for (; i < n; i++)
		(*pdst++) = (mlib_f32)(*psrc++);
#endif /* _NO_LONGLONG */

}

/* *********************************************************** */

void
mlib_SignalFIR_f32_to_s16(
    mlib_s16 *pdst,
    mlib_f32 *psrc,
    mlib_s32 n)
{
	mlib_s32 i;

#ifndef MLIB_USE_FTOI_CLAMPING
	for (i = 0; i < n; i++) {
		CLAMP_S16(pdst[i], psrc[i]);
	}

#else /* MLIB_USE_FTOI_CLAMPING */

#ifdef _NO_LONGLONG
	mlib_u32 data;

	if (((mlib_addr)pdst & 3) != 0) {
		(*pdst++) = ((mlib_s32)(*psrc++)) >> 16;
		n--;
	}

	for (i = 0; i < n - 1; i += 2) {
		data = ((mlib_u32)(mlib_s32)psrc[0]) & ~0xFFFF;
		data += (((mlib_u32)(mlib_s32)psrc[1]) >> 16);

		*(mlib_u32 *)pdst = data;
		psrc += 2;
		pdst += 2;
	}

	if (i < n)
		*pdst = ((mlib_s32)*psrc) >> 16;

#else /* _NO_LONGLONG */

	mlib_u64 data;

	for (i = 0; i < n && ((mlib_addr)pdst & 7) != 0; i++) {
		(*pdst++) = ((mlib_s32)(*psrc++)) >> 16;
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < n - 3; i += 4) {
		data = ((mlib_u32)(mlib_s32)psrc[0]) & ~0xFFFF;
		data += (((mlib_u32)(mlib_s32)psrc[1]) >> 16);
		data <<= 16;
		data += (((mlib_u32)(mlib_s32)psrc[2]) >> 16);
		data <<= 16;
		data += (((mlib_u32)(mlib_s32)psrc[3]) >> 16);

		*(mlib_u64 *)pdst = data;
		psrc += 4;
		pdst += 4;
	}

	for (; i < n; i++) {
		(*pdst++) = ((mlib_s32)(*psrc++)) >> 16;
	}

#endif /* _NO_LONGLONG */
#endif /* MLIB_USE_FTOI_CLAMPING */
}

/* *********************************************************** */

void
mlib_SignalMerge_D64_F32(
    mlib_f32 *pdst,
    mlib_d64 *psrc,
    mlib_s32 n,
    mlib_s32 factor)
{
	mlib_s32 i, j;
	mlib_d64 *sl = psrc;
	mlib_f32 *dl = pdst;

	if (factor == 2) {
		mlib_d64 *psrc1 = psrc + n;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			CLAMP_F32(pdst[2 * i], psrc[i]);
			CLAMP_F32(pdst[2 * i + 1], psrc1[i]);
		}

		return;
	}

	for (j = 0; j < n; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < factor; i++) {
			CLAMP_F32(pdst[i], psrc[i * n]);
		}

		psrc = sl += 1;
		pdst = dl += factor;
	}
}

/* *********************************************************** */

void
mlib_SignalMerge_D64_F32S(
    mlib_f32 *pdst,
    mlib_d64 *psrc,
    mlib_s32 n,
    mlib_s32 factor)
{
	mlib_s32 i, j;
	mlib_d64 *sl = psrc;
	mlib_f32 *dl = pdst;

	factor *= 2;
	n *= 2;

	if (factor == 2 * 2) {
		mlib_d64 *psrc1 = psrc + n;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i += 2) {
			CLAMP_F32(pdst[2 * i], psrc[i]);
			CLAMP_F32(pdst[2 * i + 1], psrc[i + 1]);
			CLAMP_F32(pdst[2 * i + 2], psrc1[i]);
			CLAMP_F32(pdst[2 * i + 3], psrc1[i + 1]);
		}

		return;
	}

	for (j = 0; j < n; j += 2) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < factor; i += 2) {
			CLAMP_F32(pdst[i], psrc[0]);
			CLAMP_F32(pdst[i + 1], psrc[1]);
			psrc += n;
		}

		psrc = sl += 2;
		pdst = dl += factor;
	}
}

/* *********************************************************** */

void
mlib_SignalMerge_D64_S16(
    mlib_s16 *pdst,
    mlib_d64 *psrc,
    mlib_s32 n,
    mlib_s32 factor)
{
	mlib_s32 i, j;
	mlib_d64 *sl = psrc;
	mlib_s16 *dl = pdst;

	if (factor == 2) {
		mlib_d64 *psrc1 = psrc + n;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			CLAMP_S16(pdst[2 * i], psrc[i]);
			CLAMP_S16(pdst[2 * i + 1], psrc1[i]);
		}

		return;
	}

	for (j = 0; j < n; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < factor; i++) {
			CLAMP_S16(pdst[i], psrc[i * n]);
		}

		psrc = sl += 1;
		pdst = dl += factor;
	}
}

/* *********************************************************** */

void
mlib_SignalMerge_D64_S16S(
    mlib_s16 *pdst,
    mlib_d64 *psrc,
    mlib_s32 n,
    mlib_s32 factor)
{
	mlib_s32 i, j;
	mlib_d64 *sl = psrc;
	mlib_s16 *dl = pdst;

	factor *= 2;
	n *= 2;

	if (factor == 2 * 2) {
		mlib_d64 *psrc1 = psrc + n;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i += 2) {
			CLAMP_S16(pdst[2 * i], psrc[i]);
			CLAMP_S16(pdst[2 * i + 1], psrc[i + 1]);
			CLAMP_S16(pdst[2 * i + 2], psrc1[i]);
			CLAMP_S16(pdst[2 * i + 3], psrc1[i + 1]);
		}

		return;
	}

	for (j = 0; j < n; j += 2) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < factor; i += 2) {
			CLAMP_S16(pdst[i], psrc[0]);
			CLAMP_S16(pdst[i + 1], psrc[1]);
			psrc += n;
		}

		psrc = sl += 2;
		pdst = dl += factor;
	}
}

/* *********************************************************** */
