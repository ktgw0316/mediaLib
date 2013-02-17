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

#pragma ident   "@(#)mlib_s_ImageThresh3.c	9.4	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageThresh3 - thresholding
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageThresh3(mlib_image       *dst,
 *                                    const mlib_image *src,
 *                                    const mlib_s32   *thresh,
 *                                    const mlib_s32   *ghigh);
 *
 * ARGUMENT
 *      dst     pointer to output image
 *      src     pointer to input image
 *      thresh  array of thresholds
 *      ghigh   array of values above thresholds
 *
 * RESTRICTION
 *      The images must have the same type, the same size, and the same number
 *      of channels.
 *      The images can have 1, 2, 3, or 4 channels.
 *      The images can be in MLIB_BYTE, MLIB_SHORT,
 *      MLIB_USHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *      If the pixel value is above the threshold for that channel,
 *      set the destination to the ghigh value for that channel.
 *      Otherwise, set the destination to the source.
 *
 *                      +- src[x][y][c]  src[x][y][c] <= thresh[c]
 *      dst[x][y][c]  = |
 *                      +- ghigh[c]      src[x][y][c] >  thresh[c]
 */


/* *********************************************************** */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>

#ifdef  __SUNPRO_C
#include <sunmedia_intrin.h>
#else /* __SUNPRO_C */
#include <emmintrin.h>
#endif /* __SUNPRO_C */

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageThresh3 = __mlib_ImageThresh3

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageThresh3) mlib_ImageThresh3
    __attribute__((weak, alias("__mlib_ImageThresh3")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static void mlib_s_ImageThresh3_U81(
    const mlib_u8 *psrc,
    mlib_s32 src_stride,
    mlib_u8 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_u8 *thresh,
    const mlib_u8 *ghigh);

static void mlib_s_ImageThresh3_U82(
    const mlib_u8 *psrc,
    mlib_s32 src_stride,
    mlib_u8 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_u8 *thresh,
    const mlib_u8 *ghigh);

static void mlib_s_ImageThresh3_U83(
    const mlib_u8 *psrc,
    mlib_s32 src_stride,
    mlib_u8 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_u8 *thresh,
    const mlib_u8 *ghigh);

static void mlib_s_ImageThresh3_U84(
    const mlib_u8 *psrc,
    mlib_s32 src_stride,
    mlib_u8 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_u8 *thresh,
    const mlib_u8 *ghigh);

static void mlib_s_ImageThresh3_S161(
    const mlib_s16 *psrc,
    mlib_s32 src_stride,
    mlib_s16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thresh,
    const mlib_s32 *ghigh);

static void mlib_s_ImageThresh3_S162(
    const mlib_s16 *psrc,
    mlib_s32 src_stride,
    mlib_s16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thresh,
    const mlib_s32 *ghigh);

static void mlib_s_ImageThresh3_S163(
    const mlib_s16 *psrc,
    mlib_s32 src_stride,
    mlib_s16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thresh,
    const mlib_s32 *ghigh);

static void mlib_s_ImageThresh3_S164(
    const mlib_s16 *psrc,
    mlib_s32 src_stride,
    mlib_s16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thresh,
    const mlib_s32 *ghigh);

static void mlib_s_ImageThresh3_U161(
    const mlib_u16 *psrc,
    mlib_s32 src_stride,
    mlib_u16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thresh,
    const mlib_s32 *ghigh);

static void mlib_s_ImageThresh3_U162(
    const mlib_u16 *psrc,
    mlib_s32 src_stride,
    mlib_u16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thresh,
    const mlib_s32 *ghigh);

static void mlib_s_ImageThresh3_U163(
    const mlib_u16 *psrc,
    mlib_s32 src_stride,
    mlib_u16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thresh,
    const mlib_s32 *ghigh);

static void mlib_s_ImageThresh3_U164(
    const mlib_u16 *psrc,
    mlib_s32 src_stride,
    mlib_u16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thresh,
    const mlib_s32 *ghigh);

static void mlib_s_ImageThresh3_S321(
    const mlib_s32 *psrc,
    mlib_s32 src_stride,
    mlib_s32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thresh,
    const mlib_s32 *ghigh);

static void mlib_s_ImageThresh3_S322(
    const mlib_s32 *psrc,
    mlib_s32 src_stride,
    mlib_s32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thresh,
    const mlib_s32 *ghigh);

static void mlib_s_ImageThresh3_S323(
    const mlib_s32 *psrc,
    mlib_s32 src_stride,
    mlib_s32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thresh,
    const mlib_s32 *ghigh);

static void mlib_s_ImageThresh3_S324(
    const mlib_s32 *psrc,
    mlib_s32 src_stride,
    mlib_s32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thresh,
    const mlib_s32 *ghigh);

/* *********************************************************** */
#define	INIT_THRESH0(n)                                         \
	thresh0 = thresh[(n)];                                      \
	ghigh0 = ghigh[(n)];                                        \

#define	INIT_THRESH1(n)                                         \
	thresh1 = thresh[(n)];                                      \
	ghigh1 = ghigh[(n)];                                        \

#define	INIT_THRESH2(n)                                         \
	thresh2 = thresh[(n)];                                      \
	ghigh2 = ghigh[(n)];                                        \

#define	INIT_THRESH3(n)                                         \
	thresh3 = thresh[(n)];                                      \
	ghigh3 = ghigh[(n)];                                        \

/* *********************************************************** */

#define	DO_THRESH(s0, th, gh)                                          \
	(((gh) & (((th) - (mlib_s32) (s0)) >> 31)) | ((s0) & ~(((th) -   \
		(mlib_s32) (s0)) >> 31)))

#define	THRESH0(s0)	DO_THRESH(s0, thresh0, ghigh0)
#define	THRESH1(s0)	DO_THRESH(s0, thresh1, ghigh1)
#define	THRESH2(s0)	DO_THRESH(s0, thresh2, ghigh2)
#define	THRESH3(s0)	DO_THRESH(s0, thresh3, ghigh3)

/* *********************************************************** */

#define	SET_Pointers_M128(dst, src)                              \
	dst = (__m128i *) ((mlib_addr)pdst_row);                     \
	src = (__m128i *) ((mlib_addr)psrc_row);
/* *********************************************************** */

#define	DO_FAST_THRESH_U8(dst, src, th_SSE2, gh_SSE2)            \
	ss = _mm_loadu_si128(src);                                   \
	_m_u8x16 = _mm_xor_si128(ss, mask80);                        \
	mask_gt_th = _mm_cmpgt_epi8(_m_u8x16, th_SSE2);              \
	dd =                                                         \
		_mm_or_si128(                                            \
		_mm_andnot_si128(mask_gt_th, ss),                        \
	    _mm_and_si128(gh_SSE2, mask_gt_th));                     \
	_mm_storeu_si128(dst, dd);                                   \
	dst++;                                                       \
	src++

/* *********************************************************** */

#define	GROSS_LOOPS_U8                                           \
	for (j = 0; j < w16; j++) {                                  \
	    DO_FAST_THRESH_U8(dataPtr, sourcePtr, _m_th, _m_gh);     \
	}
/* *********************************************************** */


void mlib_s_ImageThresh3_U81(
    const mlib_u8 *psrc,
    mlib_s32 src_stride,
    mlib_u8 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_u8 *thresh,
    const mlib_u8 *ghigh)
{
	mlib_s32 i, j, w16 = width >> 4;
	mlib_s32 thresh0 = thresh[0], ghigh0 = ghigh[0];
	const mlib_u8 *psrc_row = psrc;
	mlib_u8 *pdst_row = pdst;
	__m128i *sourcePtr, *dataPtr, ss, dd;
	__m128i _m_th = _mm_loadu_si128((__m128i *) thresh);
	__m128i _m_gh = _mm_loadu_si128((__m128i *) ghigh);
	__m128i _m_u8x16, mask_gt_th, u8_zero, s;
	__m128i mask80 = _mm_set1_epi32(0x80808080);

	_m_th = _mm_xor_si128(_m_th, mask80);
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (i = 0; i < height; i++) {
		SET_Pointers_M128(dataPtr, sourcePtr);
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		GROSS_LOOPS_U8;

		for (j = (w16 << 4); j < width; j++) {
			pdst_row[j] = THRESH0(psrc_row[j]);
		}

		psrc_row += src_stride;
		pdst_row += dst_stride;
	}

}

/* *********************************************************** */

void
mlib_s_ImageThresh3_U82(
    const mlib_u8 *psrc,
    mlib_s32 src_stride,
    mlib_u8 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_u8 *thresh,
    const mlib_u8 *ghigh)
{
	mlib_s32 i, j, w16 = ((width <<= 1) >> 4);
	mlib_s32 thresh0 = thresh[0], thresh1 = thresh[1];
	mlib_s32 ghigh0 = ghigh[0], ghigh1 = ghigh[1];
	const mlib_u8 *psrc_row = psrc;
	mlib_u8 *pdst_row = pdst;

	__m128i *dataPtr, *sourcePtr, ss, dd;
	__m128i _m_th = _mm_loadu_si128((__m128i *) thresh);
	__m128i _m_gh = _mm_loadu_si128((__m128i *) ghigh);
	__m128i _m_u8x16, mask_gt_th, u8_zero, s;
	__m128i mask80 = _mm_set1_epi32((int)0x80808080);

	_m_th = _mm_xor_si128(_m_th, mask80);
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (i = 0; i < height; i++) {
		SET_Pointers_M128(dataPtr, sourcePtr);
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		GROSS_LOOPS_U8;
		for (j = w16 << 4; j < width; j += 2) {
			pdst_row[j + 0] = THRESH0(psrc_row[j]);
			pdst_row[j + 1] = THRESH1(psrc_row[j + 1]);
		}

		psrc_row += src_stride;
		pdst_row += dst_stride;
	}
}

/* *********************************************************** */

void
mlib_s_ImageThresh3_U83(
    const mlib_u8 *psrc,
    mlib_s32 src_stride,
    mlib_u8 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_u8 *thresh,
    const mlib_u8 *ghigh)
{
	mlib_s32 i, j, w16 = ((width *= 3) >> 4);
	mlib_s32 thresh0 = thresh[0], thresh1 = thresh[1], thresh2 = thresh[2];
	mlib_s32 ghigh0 = ghigh[0], ghigh1 = ghigh[1], ghigh2 = ghigh[2];
	const mlib_u8 *psrc_row = psrc;
	mlib_u8 *pdst_row = pdst;

	__m128i *dataPtr, *sourcePtr, dd, ss;
	__m128i _m_th0 = _mm_loadu_si128(((__m128i *) thresh) + 0);
	__m128i _m_gh0 = _mm_loadu_si128(((__m128i *) ghigh) + 0);
	__m128i _m_th1 = _mm_loadu_si128(((__m128i *) thresh) + 1);
	__m128i _m_gh1 = _mm_loadu_si128(((__m128i *) ghigh) + 1);
	__m128i _m_th2 = _mm_loadu_si128(((__m128i *) thresh) + 2);
	__m128i _m_gh2 = _mm_loadu_si128(((__m128i *) ghigh) + 2);
	__m128i _m_u8x16, mask_gt_th;
	__m128i mask80 = _mm_set1_epi32((int)0x80808080);


	_m_th0 = _mm_xor_si128(_m_th0, mask80);
	_m_th1 = _mm_xor_si128(_m_th1, mask80);
	_m_th2 = _mm_xor_si128(_m_th2, mask80);
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (i = 0; i < height; i++) {
		SET_Pointers_M128(dataPtr, sourcePtr);
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (j = 0; j < (w16 - 2); j += 3) {

			DO_FAST_THRESH_U8(dataPtr, sourcePtr,
			    _m_th0, _m_gh0);
			DO_FAST_THRESH_U8(dataPtr, sourcePtr,
			    _m_th1, _m_gh1);
			DO_FAST_THRESH_U8(dataPtr, sourcePtr,
			    _m_th2, _m_gh2);
		}
		for (j <<= 4; j < width; j += 3) {
			pdst_row[j + 0] = THRESH0(psrc_row[j + 0]);
			pdst_row[j + 1] = THRESH1(psrc_row[j + 1]);
			pdst_row[j + 2] = THRESH2(psrc_row[j + 2]);
		}

		psrc_row += src_stride, pdst_row += dst_stride;
	}
}

/* *********************************************************** */

void
mlib_s_ImageThresh3_U84(
    const mlib_u8 *psrc,
    mlib_s32 src_stride,
    mlib_u8 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_u8 *thresh,
    const mlib_u8 *ghigh)
{
	mlib_s32 i, j, w16 = ((width <<= 2) >> 4);
	mlib_s32 thresh0 = thresh[0], thresh1 = thresh[1], thresh2 =
	    thresh[2], thresh3 = thresh[3];
	mlib_s32 ghigh0 = ghigh[0], ghigh1 = ghigh[1], ghigh2 =
	    ghigh[2], ghigh3 = ghigh[3];
	const mlib_u8 *psrc_row = psrc;
	mlib_u8 *pdst_row = pdst;

	__m128i *dataPtr, *sourcePtr, ss, dd;
	__m128i _m_th = _mm_loadu_si128((__m128i *) thresh);
	__m128i _m_gh = _mm_loadu_si128((__m128i *) ghigh);
	__m128i _m_u8x16, mask_gt_th;
	__m128i mask80 = _mm_set1_epi32((int)0x80808080);

	_m_th = _mm_xor_si128(_m_th, mask80);
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (i = 0; i < height; i++) {
		SET_Pointers_M128(dataPtr, sourcePtr);
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		GROSS_LOOPS_U8;

		for (j <<= 4; j < width; j += 4) {
			pdst_row[j + 0] = THRESH0(psrc_row[j + 0]);
			pdst_row[j + 1] = THRESH1(psrc_row[j + 1]);
			pdst_row[j + 2] = THRESH2(psrc_row[j + 2]);
			pdst_row[j + 3] = THRESH3(psrc_row[j + 3]);
		}

		psrc_row += src_stride, pdst_row += dst_stride;
	}
}

/* *********************************************************** */
#define	DO_FAST_THRESH_S16(dst, src, th_SSE2, gh_SSE2)             \
	_m_16x8 = _mm_loadu_si128(src);                                \
	mask_gt_th = _mm_cmpgt_epi16(_m_16x8, th_SSE2);                \
	dd =                                                           \
		_mm_or_si128(                                              \
		_mm_andnot_si128(mask_gt_th, _m_16x8),                     \
	    _mm_and_si128(gh_SSE2, mask_gt_th));                       \
	_mm_storeu_si128(dst, dd);                                     \
	dst++;                                                         \
	src++


/* *********************************************************** */

#define	GROSS_LOOPS_S16                                            \
	for (j = 0; j < w7; j += 8) {                                  \
	    DO_FAST_THRESH_S16(dataPtr, sourcePtr, _m_th, _m_gh);      \
	}
/* *********************************************************** */

void
mlib_s_ImageThresh3_S161(
    const mlib_s16 *psrc,
    mlib_s32 src_stride,
    mlib_s16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_s32 *thresh,
    const mlib_s32 *ghigh)
{
	mlib_s32 i, j, k, w7 = width - 7;
	mlib_s32 thresh0 = thresh[0],
		ghigh0 = ghigh[0];
	const mlib_s16 *psrc_row = psrc;
	mlib_s16 *pdst_row = pdst;
	__m128i *dataPtr, *sourcePtr, dd;
	__m128i _m_th = _mm_set1_epi16(thresh[0]);
	__m128i _m_gh = _mm_set1_epi16(ghigh[0]);
	__m128i _m_16x8, mask_gt_th;
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (i = 0; i < height; i++) {

		SET_Pointers_M128(dataPtr, sourcePtr);
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		GROSS_LOOPS_S16;

		for (; j < width; j++)
			pdst_row[j] = THRESH0(psrc_row[j]);

		psrc_row += (src_stride >> 1);
		pdst_row += (dst_stride >> 1);
	}
}

/* *********************************************************** */

void
mlib_s_ImageThresh3_S162(
    const mlib_s16 *psrc,
    mlib_s32 src_stride,
    mlib_s16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_s32 *thresh,
    const mlib_s32 *ghigh)
{
	mlib_s32 i, j, k, w7;
	mlib_s32 thresh0 = thresh[0], thresh1 = thresh[1];
	mlib_s32 ghigh0 = ghigh[0], ghigh1 = ghigh[1];
	const mlib_s16 *psrc_row = psrc;
	mlib_s16 *pdst_row = pdst;

	mlib_s32 th = (thresh1 << 16) | (thresh0 & 0xFFFF);
	mlib_s32 gh = (ghigh1 << 16) | (ghigh0 & 0xFFFF);

	__m128i *dataPtr, *sourcePtr, dd;
	__m128i _m_th = _mm_set1_epi32(th);
	__m128i _m_gh = _mm_set1_epi32(gh);
	__m128i _m_16x8, mask_gt_th;

	width <<= 1;
	w7 = width - 7;
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (i = 0; i < height; i++) {

		SET_Pointers_M128(dataPtr, sourcePtr);
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		GROSS_LOOPS_S16;

		for (; j < width; j += 2) {
			pdst_row[j + 0] = THRESH0(psrc_row[j]);
			pdst_row[j + 1] = THRESH1(psrc_row[j + 1]);
		}

		psrc_row += (src_stride >> 1);
		pdst_row += (dst_stride >> 1);
	}
}

/* *********************************************************** */

void
mlib_s_ImageThresh3_S163(
    const mlib_s16 *psrc,
    mlib_s32 src_stride,
    mlib_s16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_s32 *thresh,
    const mlib_s32 *ghigh)
{
	mlib_s32 i, j, k;
	mlib_s32 thresh0, thresh1, thresh2;
	mlib_s32 ghigh0, ghigh1, ghigh2;
	const mlib_s16 *psrc_row = psrc;
	mlib_s16 *pdst_row = pdst;
	__m128i *dataPtr, *sourcePtr, dd;
	__m128i _m_th0, _m_th1, _m_th2;
	__m128i _m_gh0, _m_gh1, _m_gh2;
	__m128i _m_16x8, mask_gt_th;

	{
		__m128i temp10_0 = _mm_cvtsi32_si128(
					(thresh[0] & 0xFFFF) |
					(thresh[1] << 16));
		__m128i temp02_0 = _mm_cvtsi32_si128(
					(thresh[2] & 0xFFFF) |
					(thresh[0] << 16));
		__m128i temp21_0 = _mm_cvtsi32_si128(
					(thresh[1] & 0xFFFF) |
					(thresh[2] << 16));
		__m128i temp10_2 = _mm_cvtsi32_si128(
					(ghigh[0] & 0xFFFF) |
					(ghigh[1] << 16));
		__m128i temp02_2 = _mm_cvtsi32_si128(
					(ghigh[2] & 0xFFFF) |
					(ghigh[0] << 16));
		__m128i temp21_2 = _mm_cvtsi32_si128(
					(ghigh[1] & 0xFFFF) |
					(ghigh[2] << 16));

		_m_th0 =
			_mm_or_si128(_mm_slli_si128(
			_mm_or_si128(_mm_slli_epi64(
			temp10_0, 32), temp21_0), 8),
			_mm_or_si128(_mm_slli_epi64(
			temp02_0, 32), temp10_0));
		_m_th1 =
			_mm_or_si128(_mm_slli_si128(
			_mm_or_si128(_mm_slli_epi64(
			temp02_0, 32), temp10_0), 8),
			_mm_or_si128(_mm_slli_epi64(
			temp21_0, 32), temp02_0));
		_m_th2 =
			_mm_or_si128(_mm_slli_si128(
			_mm_or_si128(_mm_slli_epi64(
			temp21_0, 32), temp02_0), 8),
			_mm_or_si128(_mm_slli_epi64(
			temp10_0, 32), temp21_0));
		_m_gh0 =
			_mm_or_si128(_mm_slli_si128(
			_mm_or_si128(_mm_slli_epi64(
			temp10_2, 32), temp21_2), 8),
			_mm_or_si128(_mm_slli_epi64(
			temp02_2, 32), temp10_2));
		_m_gh1 =
			_mm_or_si128(_mm_slli_si128(
			_mm_or_si128(_mm_slli_epi64(
			temp02_2, 32), temp10_2), 8),
			_mm_or_si128(_mm_slli_epi64(
			temp21_2, 32), temp02_2));
		_m_gh2 =
			_mm_or_si128(_mm_slli_si128(
			_mm_or_si128(_mm_slli_epi64(
			temp21_2, 32), temp02_2), 8),
			_mm_or_si128(_mm_slli_epi64(
			temp10_2, 32), temp21_2));
	}

	INIT_THRESH0(0);
	INIT_THRESH1(1);
	INIT_THRESH2(2);

	width = width * 3;
	src_stride >>= 1;
	dst_stride >>= 1;
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (i = 0; i < height; i++) {
		SET_Pointers_M128(dataPtr, sourcePtr);
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (j = 0; j < (width - 23); j += 24) {
			DO_FAST_THRESH_S16(dataPtr, sourcePtr,
			    _m_th0, _m_gh0);
			DO_FAST_THRESH_S16(dataPtr, sourcePtr,
			    _m_th1, _m_gh1);
			DO_FAST_THRESH_S16(dataPtr, sourcePtr,
			    _m_th2, _m_gh2);
		}

		for (; j < width; j += 3) {
			pdst_row[j] = THRESH0(psrc_row[j]);
			pdst_row[j + 1] = THRESH1(psrc_row[j + 1]);
			pdst_row[j + 2] = THRESH2(psrc_row[j + 2]);
		}
		psrc_row += src_stride;
		pdst_row += dst_stride;
	}
}

/* *********************************************************** */

void
mlib_s_ImageThresh3_S164(
    const mlib_s16 *psrc,
    mlib_s32 src_stride,
    mlib_s16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_s32 *thresh,
    const mlib_s32 *ghigh)
{
	mlib_s32 i, j, k, w7;
	mlib_s32 thresh0 = thresh[0], thresh1 = thresh[1], thresh2 =
	    thresh[2], thresh3 = thresh[3];
	mlib_s32 ghigh0 = ghigh[0], ghigh1 = ghigh[1], ghigh2 =
	    ghigh[2], ghigh3 = ghigh[3];
	__m128i *dataPtr, *sourcePtr, dd;
	const mlib_s16 *psrc_row = psrc;
	mlib_s16 *pdst_row = pdst;

	__m128i _m_th =
	    _mm_or_si128(_mm_slli_epi64(
		_mm_cvtsi32_si128((thresh3 << 16) | (thresh2 & 0xffff)), 32),
		_mm_cvtsi32_si128((thresh1 << 16) | (thresh0 & 0xffff)));
	_m_th = _mm_unpacklo_epi64(_m_th, _m_th);

	__m128i _m_gh =
	    _mm_or_si128(_mm_slli_epi64(
		_mm_cvtsi32_si128((ghigh3 << 16) | (ghigh2 & 0xffff)), 32),
		_mm_cvtsi32_si128((ghigh1 << 16) | (ghigh0 & 0xffff)));
	_m_gh = _mm_unpacklo_epi64(_m_gh, _m_gh);

	__m128i _m_16x8;
	__m128i mask_gt_th;

	width <<= 2;
	w7 = width - 7;
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (i = 0; i < height; i++) {

		SET_Pointers_M128(dataPtr, sourcePtr);
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		GROSS_LOOPS_S16;

		for (; j < width; j += 4) {
			pdst_row[j] = THRESH0(psrc_row[j]);
			pdst_row[j + 1] = THRESH1(psrc_row[j + 1]);
			pdst_row[j + 2] = THRESH2(psrc_row[j + 2]);
			pdst_row[j + 3] = THRESH3(psrc_row[j + 3]);
		}

		psrc_row += (src_stride >> 1);
		pdst_row += (dst_stride >> 1);
	}
}

/* *********************************************************** */
#define	DO_FAST_THRESH_U16(dst, src, th_SSE2, gh_SSE2)            \
	ss = _mm_loadu_si128(src);                                    \
	_m_16x8 = _mm_xor_si128(ss, mask8000);                        \
	mask_gt_th = _mm_cmpgt_epi16(_m_16x8, th_SSE2);               \
	dd =                                                          \
		_mm_or_si128(                                             \
		_mm_andnot_si128(mask_gt_th, ss),                    \
	    _mm_and_si128(gh_SSE2, mask_gt_th));                      \
	_mm_storeu_si128(dst, dd);                                    \
	dst++;                                                        \
	src++
/* *********************************************************** */

#define	GROSS_LOOPS_U16                                           \
	for (j = 0; j < w7; j += 8) {                                 \
	    DO_FAST_THRESH_U16(dataPtr, sourcePtr, _m_th, _m_gh);     \
	}
/* *********************************************************** */


void
mlib_s_ImageThresh3_U161(
    const mlib_u16 *psrc,
    mlib_s32 src_stride,
    mlib_u16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_s32 *thresh,
    const mlib_s32 *ghigh)
{
	mlib_s32 i, j, k, w7 = width - 7;
	mlib_s32 thresh0 = thresh[0],
			ghigh0 = ghigh[0];
	const mlib_u16 *psrc_row = psrc;
	mlib_u16 *pdst_row = pdst;
	__m128i *dataPtr, *sourcePtr, ss, dd;
	__m128i _m_th = _mm_set1_epi16(thresh[0]);
	__m128i _m_gh = _mm_set1_epi16(ghigh[0]);
	__m128i _m_16x8, mask_gt_th;
	__m128i mask8000 = _mm_set1_epi32(0x80008000);

	_m_th = _mm_xor_si128(_m_th, mask8000);
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (i = 0; i < height; i++) {

		SET_Pointers_M128(dataPtr, sourcePtr);
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		GROSS_LOOPS_U16;

		for (; j < width; j++)
			pdst_row[j] = THRESH0(psrc_row[j]);

		psrc_row += (src_stride >> 1);
		pdst_row += (dst_stride >> 1);
	}

}

/* *********************************************************** */

void
mlib_s_ImageThresh3_U162(
    const mlib_u16 *psrc,
    mlib_s32 src_stride,
    mlib_u16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_s32 *thresh,
    const mlib_s32 *ghigh)
{
	mlib_s32 i, j, k, w7;
	mlib_s32 thresh0 = thresh[0], thresh1 = thresh[1];
	mlib_s32 ghigh0 = ghigh[0], ghigh1 = ghigh[1];
	const mlib_u16 *psrc_row = psrc;
	mlib_u16 *pdst_row = pdst;

	mlib_s32 th = (thresh1 << 16) | (thresh0 & 0xFFFF);
	mlib_s32 gh = (ghigh1 << 16) | (ghigh0 & 0xFFFF);

	__m128i *dataPtr, *sourcePtr, ss, dd;
	__m128i _m_th = _mm_set1_epi32(th);
	__m128i _m_gh = _mm_set1_epi32(gh);
	__m128i _m_16x8, mask_gt_th;
	__m128i mask8000 = _mm_set1_epi32(0x80008000);

	_m_th = _mm_xor_si128(_m_th, mask8000);

	width <<= 1;
	w7 = width - 7;
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (i = 0; i < height; i++) {

		SET_Pointers_M128(dataPtr, sourcePtr);
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		GROSS_LOOPS_U16;

		for (; j < width; j += 2) {
			pdst_row[j + 0] = THRESH0(psrc_row[j]);
			pdst_row[j + 1] = THRESH1(psrc_row[j + 1]);
		}

		psrc_row += (src_stride >> 1);
		pdst_row += (dst_stride >> 1);
	}

}

/* *********************************************************** */

void
mlib_s_ImageThresh3_U163(
    const mlib_u16 *psrc,
    mlib_s32 src_stride,
    mlib_u16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_s32 *thresh,
    const mlib_s32 *ghigh)
{
	mlib_s32 i, j, k;
	mlib_s32 thresh0, thresh1, thresh2;
	mlib_s32 ghigh0, ghigh1, ghigh2;
	const mlib_u16 *psrc_row = psrc;
	mlib_u16 *pdst_row = pdst;
	__m128i *dataPtr, *sourcePtr, ss, dd;
	__m128i _m_th0, _m_th1, _m_th2;
	__m128i _m_gh0, _m_gh1, _m_gh2;
	__m128i _m_16x8, mask_gt_th;
	__m128i mask8000 = _mm_set1_epi32((int)0x80008000);

	{
		mlib_s32 temp10_0 = (thresh[0] & 0xFFFF) | (thresh[1] << 16);
		mlib_s32 temp02_0 = (thresh[2] & 0xFFFF) | (thresh[0] << 16);
		mlib_s32 temp21_0 = (thresh[1] & 0xFFFF) | (thresh[2] << 16);
		mlib_s32 temp10_2 = (ghigh[0] & 0xFFFF) | (ghigh[1] << 16);
		mlib_s32 temp02_2 = (ghigh[2] & 0xFFFF) | (ghigh[0] << 16);
		mlib_s32 temp21_2 = (ghigh[1] & 0xFFFF) | (ghigh[2] << 16);

		_m_th0 = _mm_setr_epi32(temp10_0, temp02_0, temp21_0, temp10_0);
		_m_th1 = _mm_setr_epi32(temp02_0, temp21_0, temp10_0, temp02_0);
		_m_th2 = _mm_setr_epi32(temp21_0, temp10_0, temp02_0, temp21_0);
		_m_gh0 = _mm_setr_epi32(temp10_2, temp02_2, temp21_2, temp10_2);
		_m_gh1 = _mm_setr_epi32(temp02_2, temp21_2, temp10_2, temp02_2);
		_m_gh2 = _mm_setr_epi32(temp21_2, temp10_2, temp02_2, temp21_2);
	}
	_m_th0 = _mm_xor_si128(_m_th0, mask8000);
	_m_th1 = _mm_xor_si128(_m_th1, mask8000);
	_m_th2 = _mm_xor_si128(_m_th2, mask8000);

	INIT_THRESH0(0);
	INIT_THRESH1(1);
	INIT_THRESH2(2);

	width = width * 3;
	src_stride >>= 1;
	dst_stride >>= 1;
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (i = 0; i < height; i++) {

		SET_Pointers_M128(dataPtr, sourcePtr);
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (j = 0; j < (width - 23); j += 24) {
			DO_FAST_THRESH_U16(dataPtr, sourcePtr,
			    _m_th0, _m_gh0);
			DO_FAST_THRESH_U16(dataPtr, sourcePtr,
			    _m_th1, _m_gh1);
			DO_FAST_THRESH_U16(dataPtr, sourcePtr,
			    _m_th2, _m_gh2);
		}

		for (; j < width; j += 3) {
			pdst_row[j] = THRESH0(psrc_row[j]);
			pdst_row[j + 1] = THRESH1(psrc_row[j + 1]);
			pdst_row[j + 2] = THRESH2(psrc_row[j + 2]);
		}
		psrc_row += src_stride;
		pdst_row += dst_stride;
	}
}
/* *********************************************************** */

void
mlib_s_ImageThresh3_U164(
    const mlib_u16 *psrc,
    mlib_s32 src_stride,
    mlib_u16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_s32 *thresh,
    const mlib_s32 *ghigh)
{
	mlib_s32 i, j, k, w7;
	mlib_s32 thresh0 = thresh[0], thresh1 = thresh[1], thresh2 =
	    thresh[2], thresh3 = thresh[3];
	mlib_s32 ghigh0 = ghigh[0], ghigh1 = ghigh[1], ghigh2 =
	    ghigh[2], ghigh3 = ghigh[3];
	__m128i *dataPtr, *sourcePtr, ss, dd;
	const mlib_u16 *psrc_row = psrc;
	mlib_u16 *pdst_row = pdst;

	__m128i _m_th =
	    _mm_or_si128(_mm_slli_epi64(
		_mm_cvtsi32_si128((thresh3 << 16) | (thresh2 & 0xffff)), 32),
		_mm_cvtsi32_si128((thresh1 << 16) | (thresh0 & 0xffff)));
	_m_th = _mm_unpacklo_epi64(_m_th, _m_th);

	__m128i _m_gh =
	    _mm_or_si128(_mm_slli_epi64(
		_mm_cvtsi32_si128((ghigh3 << 16) | (ghigh2 & 0xffff)), 32),
		_mm_cvtsi32_si128((ghigh1 << 16) | (ghigh0 & 0xffff)));
	_m_gh = _mm_unpacklo_epi64(_m_gh, _m_gh);

	__m128i mask8000 = _mm_set1_epi32(0x80008000);

	_m_th = _mm_xor_si128(_m_th, mask8000);


	__m128i _m_16x8;
	__m128i mask_gt_th;

	width <<= 2;
	w7 = width - 7;
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (i = 0; i < height; i++) {

		SET_Pointers_M128(dataPtr, sourcePtr);
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		GROSS_LOOPS_U16;

		for (; j < width; j += 4) {
			pdst_row[j] = THRESH0(psrc_row[j]);
			pdst_row[j + 1] = THRESH1(psrc_row[j + 1]);
			pdst_row[j + 2] = THRESH2(psrc_row[j + 2]);
			pdst_row[j + 3] = THRESH3(psrc_row[j + 3]);
		}

		psrc_row += (src_stride >> 1);
		pdst_row += (dst_stride >> 1);
	}
}
/* *********************************************************** */

#define	DO_THRESH_S32(s0, th, gh)                         \
	((s0) > (th)) ? (gh) : (s0)

#define	THRESH0_S32(s0)	DO_THRESH_S32(s0, thresh0, ghigh0)
#define	THRESH1_S32(s0)	DO_THRESH_S32(s0, thresh1, ghigh1)
#define	THRESH2_S32(s0)	DO_THRESH_S32(s0, thresh2, ghigh2)
#define	THRESH3_S32(s0)	DO_THRESH_S32(s0, thresh3, ghigh3)

/* *********************************************************** */
#define	DO_FAST_THRESH_S32(dst, src, th_SSE2, gh_SSE2)             \
	_m_32x4 = _mm_loadu_si128(src);                                \
	mask_gt_th = _mm_cmpgt_epi32(_m_32x4, th_SSE2);                \
	dd =                                                           \
		_mm_or_si128(                                              \
		_mm_andnot_si128(mask_gt_th, _m_32x4),                     \
	    _mm_and_si128(gh_SSE2, mask_gt_th));                       \
	_mm_storeu_si128(dst, dd);                                     \
	dst++;                                                         \
	src++
/* *********************************************************** */

#define	GROSS_LOOPS_S32                                            \
	for (j = 0; j < w3; j += 4) {                                  \
	    DO_FAST_THRESH_S32(dataPtr, sourcePtr, _m_th, _m_gh);      \
	}
/* *********************************************************** */


void
mlib_s_ImageThresh3_S321(
    const mlib_s32 *psrc,
    mlib_s32 src_stride,
    mlib_s32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_s32 *thresh,
    const mlib_s32 *ghigh)
{
	mlib_s32 i, j, k, w3 = width - 3;
	const mlib_s32 *psrc_row = psrc;
	mlib_s32 thresh0, ghigh0;
	mlib_s32 *pdst_row = pdst;
	__m128i *dataPtr, *sourcePtr, dd;
	__m128i _m_th = _mm_set1_epi32(thresh[0]);
	__m128i _m_gh = _mm_set1_epi32(ghigh[0]);
	__m128i _m_32x4, mask_gt_th;

	src_stride >>= 2;
	dst_stride >>= 2;

	INIT_THRESH0(0);
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (i = 0, j = 0; i < height; i++, j = 0) {

		SET_Pointers_M128(dataPtr, sourcePtr);
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		GROSS_LOOPS_S32;
		for (; j < width; j++)
			pdst_row[j] = THRESH0_S32(psrc_row[j]);

		psrc_row += src_stride;
		pdst_row += dst_stride;
	}
}

/* *********************************************************** */

void
mlib_s_ImageThresh3_S322(
    const mlib_s32 *psrc,
    mlib_s32 src_stride,
    mlib_s32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_s32 *thresh,
    const mlib_s32 *ghigh)
{
	mlib_s32 i, j, k, w3 = width;
	const mlib_s32 *psrc_row = psrc;
	mlib_s32 *pdst_row = pdst;
	mlib_s32 thresh0, thresh1;
	mlib_s32 ghigh0, ghigh1;

	__m128i *dataPtr, *sourcePtr, dd;
	__m128i _m_th = _mm_setr_epi32(
			thresh[0], thresh[1], thresh[0], thresh[1]);
	__m128i _m_gh = _mm_setr_epi32(
			ghigh[0], ghigh[1], ghigh[0], ghigh[1]);
	__m128i _m_32x4, mask_gt_th;

	src_stride >>= 2;
	dst_stride >>= 2;
	width <<= 1;
	w3 = width - 3;

	INIT_THRESH0(0);
	INIT_THRESH1(1);
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (i = 0, j = 0; i < height; i++, j = 0) {

		SET_Pointers_M128(dataPtr, sourcePtr);
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		GROSS_LOOPS_S32;
		for (; j < width; j += 2) {
			pdst_row[j + 0] = THRESH0_S32(psrc_row[j + 0]);
			pdst_row[j + 1] = THRESH1_S32(psrc_row[j + 1]);
		}

		psrc_row += src_stride;
		pdst_row += dst_stride;
	}
}

/* *********************************************************** */

void
mlib_s_ImageThresh3_S323(
    const mlib_s32 *psrc,
    mlib_s32 src_stride,
    mlib_s32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_s32 *thresh,
    const mlib_s32 *ghigh)
{
	mlib_s32 i, j, k;
	mlib_s32 thresh0, thresh1, thresh2;
	mlib_s32 ghigh0, ghigh1, ghigh2;
	const mlib_s32 *psrc_row = psrc;
	mlib_s32 *pdst_row = pdst;
	__m128i *dataPtr, *sourcePtr, dd;
	__m128i _m_th0 = _mm_setr_epi32(thresh[0], thresh[1],
						thresh[2], thresh[0]);
	__m128i _m_th1 = _mm_setr_epi32(thresh[1], thresh[2],
						thresh[0], thresh[1]);
	__m128i _m_th2 = _mm_setr_epi32(thresh[2], thresh[0],
						thresh[1], thresh[2]);
	__m128i _m_gh0 = _mm_setr_epi32(ghigh[0], ghigh[1],
						ghigh[2], ghigh[0]);
	__m128i _m_gh1 = _mm_setr_epi32(ghigh[1], ghigh[2],
						ghigh[0], ghigh[1]);
	__m128i _m_gh2 = _mm_setr_epi32(ghigh[2], ghigh[0],
						ghigh[1], ghigh[2]);
	__m128i _m_32x4, mask_gt_th;

	src_stride >>= 2;
	dst_stride >>= 2;

	INIT_THRESH0(0);
	INIT_THRESH1(1);
	INIT_THRESH2(2);
	width = width * 3;
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (i = 0; i < height; i++) {

		SET_Pointers_M128(dataPtr, sourcePtr);
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (j = 0; j < (width - 11); j += 12) {
			DO_FAST_THRESH_S32(dataPtr, sourcePtr,
			    _m_th0, _m_gh0);
			DO_FAST_THRESH_S32(dataPtr, sourcePtr,
			    _m_th1, _m_gh1);
			DO_FAST_THRESH_S32(dataPtr, sourcePtr,
			    _m_th2, _m_gh2);
		}

		for (; j < width; j += 3) {
			pdst_row[j] = THRESH0_S32(psrc_row[j]);
			pdst_row[j + 1] = THRESH1_S32(psrc_row[j + 1]);
			pdst_row[j + 2] = THRESH2_S32(psrc_row[j + 2]);
		}

		psrc_row += src_stride;
		pdst_row += dst_stride;
	}
}

/* *********************************************************** */

void
mlib_s_ImageThresh3_S324(
    const mlib_s32 *psrc,
    mlib_s32 src_stride,
    mlib_s32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_s32 *thresh,
    const mlib_s32 *ghigh)
{
	mlib_s32 i, j, k, w3;
	const mlib_s32 *psrc_row = psrc;
	mlib_s32 *pdst_row = pdst;

	__m128i *dataPtr, *sourcePtr, dd;
	__m128i _m_th = _mm_setr_epi32(
			thresh[0], thresh[1], thresh[2], thresh[3]);
	__m128i _m_gh = _mm_setr_epi32(
			ghigh[0], ghigh[1], ghigh[2], ghigh[3]);
	__m128i _m_32x4, mask_gt_th;

	src_stride >>= 2;
	dst_stride >>= 2;
	width <<= 2;
	w3 = width - 3;
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (i = 0; i < height; i++) {

		SET_Pointers_M128(dataPtr, sourcePtr);
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		GROSS_LOOPS_S32;
		psrc_row += src_stride;
		pdst_row += dst_stride;
	}
}
/* *********************************************************** */
mlib_status
__mlib_ImageThresh3(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *thresh,
    const mlib_s32 *ghigh)
{
	mlib_s32 sstride, dstride, width, height, nchan;
	mlib_type dtype, stype;
	void *psrc, *pdst;
	mlib_s32 th[4], tl[4], gh[4];
	mlib_s32 i, j;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_SIZE_EQUAL(src, dst);
	MLIB_IMAGE_CHAN_EQUAL(src, dst);
	MLIB_IMAGE_TYPE_DSTBIT_OR_EQ(src, dst);

	stype = mlib_ImageGetType(src);
	dtype = mlib_ImageGetType(dst);
	nchan = mlib_ImageGetChannels(src);
	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	sstride = mlib_ImageGetStride(src);
	dstride = mlib_ImageGetStride(dst);
	psrc = mlib_ImageGetData(src);
	pdst = mlib_ImageGetData(dst);

	if (thresh == NULL)
		return (MLIB_NULLPOINTER);
	if (ghigh == NULL)
		return (MLIB_NULLPOINTER);

	if (dtype == MLIB_BYTE) {
		mlib_u8 th_u8[48], gh_u8[48];

		for (i = 0; i < nchan; i++) {
			if (thresh[i] < MLIB_U8_MIN) {
				for (i = 0; i < nchan; i++) {
					th[i] = MLIB_U8_MAX;
					gh[i] = ghigh[i] & 255;

					if (thresh[i] < MLIB_U8_MIN)
						tl[i] = MLIB_U8_MIN;
					else if (thresh[i] >= MLIB_U8_MAX)
						gh[i] = tl[i] = MLIB_U8_MAX;
					else
						tl[i] = thresh[i] + 1;
				}

				return __mlib_ImageThresh5(dst, src, th, tl,
				    gh);
			}
		}

		for (i = 0; i < nchan; i++) {
			gh[i] = ghigh[i] & 255;
			th[i] = thresh[i];
			if (thresh[i] < 0) {
				th[i] = 0;
			} else if (thresh[i] > MLIB_U8_MAX) {
				th[i] = MLIB_U8_MAX;
			}
		}

		for (j = 0; j < 48; j += i) {
			for (i = 0; i < nchan; i++) {
				gh_u8[j + i] = (mlib_u8)gh[i];
				th_u8[j + i] = (mlib_u8)th[i];
			}
		}

		switch (nchan) {

		case 1:
			mlib_s_ImageThresh3_U81((mlib_u8 *)psrc,
				sstride, (mlib_u8 *)pdst, dstride,
				width, height, th_u8, gh_u8);
			break;
		case 2:
			mlib_s_ImageThresh3_U82((mlib_u8 *)psrc,
				sstride, (mlib_u8 *)pdst, dstride,
				width, height, th_u8, gh_u8);
			break;
		case 3:
			mlib_s_ImageThresh3_U83((mlib_u8 *)psrc,
				sstride, (mlib_u8 *)pdst, dstride,
				width, height, th_u8, gh_u8);
			break;
		case 4:
			mlib_s_ImageThresh3_U84((mlib_u8 *)psrc,
				sstride, (mlib_u8 *)pdst, dstride,
				width, height, th_u8, gh_u8);
			break;
		}
		return (MLIB_SUCCESS);

	} else if (dtype == MLIB_SHORT) {
		for (i = 0; i < nchan; i++) {
			if (thresh[i] < MLIB_S16_MIN) {
				for (i = 0; i < nchan; i++) {
					th[i] = MLIB_S16_MAX;
					gh[i] = (ghigh[i] << 16) >> 16;

					if (thresh[i] < MLIB_S16_MIN)
						tl[i] = MLIB_S16_MIN;
					else if (thresh[i] >= MLIB_S16_MAX)
						gh[i] = tl[i] = MLIB_S16_MAX;
					else
						tl[i] = thresh[i] + 1;
				}

				return __mlib_ImageThresh5(dst, src, th, tl,
				    gh);
			}
		}

		for (i = 0; i < nchan; i++) {
			gh[i] = (ghigh[i] << 16) >> 16;
			th[i] = thresh[i];
			if (thresh[i] > MLIB_S16_MAX)
				th[i] = MLIB_S16_MAX;
			else if (thresh[i] < MLIB_S16_MIN) {
				th[i] = MLIB_S16_MIN;
			}
		}

		switch (nchan) {
		case 1:
			mlib_s_ImageThresh3_S161((mlib_s16 *)psrc,
				sstride, (mlib_s16 *)pdst, dstride,
				width, height, th, gh);
			break;
		case 2:
			mlib_s_ImageThresh3_S162((mlib_s16 *)psrc,
				sstride, (mlib_s16 *)pdst, dstride,
				width, height, th, gh);
			break;
		case 3:
			mlib_s_ImageThresh3_S163((mlib_s16 *)psrc,
				sstride, (mlib_s16 *)pdst, dstride,
				width, height, th, gh);
			break;
		case 4:
			mlib_s_ImageThresh3_S164((mlib_s16 *)psrc,
				sstride, (mlib_s16 *)pdst, dstride,
				width, height, th, gh);
			break;
		}
		return (MLIB_SUCCESS);

	} else if (dtype == MLIB_USHORT) {
		for (i = 0; i < nchan; i++) {
			if (thresh[i] < MLIB_U16_MIN) {
				for (i = 0; i < nchan; i++) {
					th[i] = MLIB_U16_MAX;
					gh[i] = (ghigh[i] << 16) >> 16;

					if (thresh[i] < MLIB_U16_MIN)
						tl[i] = MLIB_U16_MIN;
					else if (thresh[i] >= MLIB_U16_MAX)
						gh[i] = tl[i] = MLIB_U16_MAX;
					else
						tl[i] = thresh[i] + 1;
				}

				return __mlib_ImageThresh5(dst, src, th, tl,
				    gh);
			}
		}

		for (i = 0; i < nchan; i++) {
			gh[i] = (ghigh[i] << 16) >> 16;
			th[i] = thresh[i];
			if (thresh[i] > MLIB_U16_MAX) {
				th[i] = MLIB_U16_MAX;
			} if (thresh[i] < MLIB_U16_MIN) {
				th[i] = MLIB_U16_MIN;
			}
		}

		switch (nchan) {
		case 1:
			mlib_s_ImageThresh3_U161(
				psrc, sstride, pdst, dstride,
				width, height, th, gh);
			break;
		case 2:
			mlib_s_ImageThresh3_U162(
				psrc, sstride, pdst, dstride,
				width, height, th, gh);
			break;
		case 3:
			mlib_s_ImageThresh3_U163(
				psrc, sstride, pdst, dstride,
				width, height, th, gh);
			break;
		case 4:
			mlib_s_ImageThresh3_U164(
				psrc, sstride, pdst, dstride,
				width, height, th, gh);
			break;
		}
		return (MLIB_SUCCESS);

	} else if (dtype == MLIB_INT) {

		switch (nchan) {
		case 1:
			mlib_s_ImageThresh3_S321(
				(mlib_s32 *)psrc, sstride,
			    (mlib_s32 *)pdst, dstride,
				width, height, thresh, ghigh);
			break;
		case 2:
			mlib_s_ImageThresh3_S322(
				(mlib_s32 *)psrc, sstride,
			    (mlib_s32 *)pdst, dstride,
				width, height, thresh, ghigh);
			break;
		case 3:
			mlib_s_ImageThresh3_S323(
				(mlib_s32 *)psrc, sstride,
			    (mlib_s32 *)pdst, dstride,
				width, height, thresh, ghigh);
			break;
		case 4:
			mlib_s_ImageThresh3_S324(
				(mlib_s32 *)psrc, sstride,
			    (mlib_s32 *)pdst, dstride,
				width, height, thresh, ghigh);
			break;
		}
		return (MLIB_SUCCESS);
	}

	return (MLIB_FAILURE);
}
/* *********************************************************** */
