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

#pragma ident   "@(#)mlib_s_ImageThresh2.c	9.4	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageThresh2 - thresholding
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageThresh2(mlib_image       *dst,
 *                                    const mlib_image *src,
 *                                    const mlib_s32   *thresh,
 *                                    const mlib_s32   *ghigh,
 *                                    const mlib_s32   *glow);
 *
 * ARGUMENT
 *      dst     pointer to output image
 *      src     pointer to input image
 *      thresh  array of thresholds
 *      ghigh   array of values above thresholds
 *      glow    array of values below thresholds
 *
 * RESTRICTION
 *      The images must have the same size, and the same number
 *      of channels.
 *      The images can have 1, 2, 3, or 4 channels.
 *      The images can be in MLIB_BYTE, MLIB_SHORT, MLIB_USHORT
 *      or MLIB_INT data type.
 *      The type of the output image can be MLIB_BIT, or the same as the
 *      type of the input image.
 *
 * DESCRIPTION
 *      If the pixel band value is above the threshold for that channel,
 *      set the destination to the ghigh value for that channel.
 *      Otherwise, set the destination to the glow value for that channel.
 *
 *                      +- glow[c]   src[x][y][c] <= thresh[c]
 *      dst[x][y][c]  = |
 *                      +- ghigh[c]  src[x][y][c] >  thresh[c]
 */

/* *********************************************************** */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#ifdef  __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageThresh2 = __mlib_ImageThresh2

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageThresh2) mlib_ImageThresh2
    __attribute__((weak, alias("__mlib_ImageThresh2")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

    void mlib_s_ImageThresh2_U81(
    const mlib_u8 *psrc,
    mlib_s32 src_stride,
    mlib_u8 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_u8 *thresh,
    const mlib_u8 *glow);
    void mlib_s_ImageThresh2_U82(
    const mlib_u8 *psrc,
    mlib_s32 src_stride,
    mlib_u8 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_u8 *thresh,
    const mlib_u8 *glow);
    void mlib_s_ImageThresh2_U83(
    const mlib_u8 *psrc,
    mlib_s32 src_stride,
    mlib_u8 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_u8 *thresh,
    const mlib_u8 *glow);
    void mlib_s_ImageThresh2_U84(
    const mlib_u8 *psrc,
    mlib_s32 src_stride,
    mlib_u8 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_u8 *thresh,
    const mlib_u8 *glow);

    void mlib_s_ImageThresh2_S161(
    const mlib_s16 *psrc,
    mlib_s32 src_stride,
    mlib_s16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_s32 *thresh,
    const mlib_s32 *glow);
    void mlib_s_ImageThresh2_S162(
    const mlib_s16 *psrc,
    mlib_s32 src_stride,
    mlib_s16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_s32 *thresh,
    const mlib_s32 *glow);
    void mlib_s_ImageThresh2_S163(
    const mlib_s16 *psrc,
    mlib_s32 src_stride,
    mlib_s16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_s32 *thresh,
    const mlib_s32 *glow);
    void mlib_s_ImageThresh2_S164(
    const mlib_s16 *psrc,
    mlib_s32 src_stride,
    mlib_s16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_s32 *thresh,
    const mlib_s32 *glow);

	void mlib_s_ImageThresh2_U161(
    const mlib_u16 *psrc,
    mlib_s32 src_stride,
    mlib_u16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_s32 *thresh,
    const mlib_s32 *glow);
	void mlib_s_ImageThresh2_U162(
    const mlib_u16 *psrc,
    mlib_s32 src_stride,
    mlib_u16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_s32 *thresh,
    const mlib_s32 *glow);
	void mlib_s_ImageThresh2_U163(
    const mlib_u16 *psrc,
    mlib_s32 src_stride,
    mlib_u16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_s32 *thresh,
    const mlib_s32 *glow);
	void mlib_s_ImageThresh2_U164(
    const mlib_u16 *psrc,
    mlib_s32 src_stride,
    mlib_u16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_s32 *thresh,
    const mlib_s32 *glow);

    void mlib_s_ImageThresh2_S321(
    const mlib_s32 *psrc,
    mlib_s32 src_stride,
    mlib_s32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_s32 *thresh,
    const mlib_s32 *glow);
    void mlib_s_ImageThresh2_S322(
    const mlib_s32 *psrc,
    mlib_s32 src_stride,
    mlib_s32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_s32 *thresh,
    const mlib_s32 *glow);
    void mlib_s_ImageThresh2_S323(
    const mlib_s32 *psrc,
    mlib_s32 src_stride,
    mlib_s32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_s32 *thresh,
    const mlib_s32 *glow);
    void mlib_s_ImageThresh2_S324(
    const mlib_s32 *psrc,
    mlib_s32 src_stride,
    mlib_s32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_s32 *thresh,
    const mlib_s32 *glow);

/* *********************************************************** */
#define	INIT_THRESH0(n)                                         \
	thresh0 = thresh[(n)];                                      \
	glow0 = glow[(n)];

#define	INIT_THRESH1(n)                                         \
	thresh1 = thresh[(n)];                                      \
	glow1  = glow[(n)];                                         \

#define	INIT_THRESH2(n)                                         \
	thresh2 = thresh[(n)];                                      \
	glow2 = glow[(n)];

#define	INIT_THRESH3(n)                                         \
	thresh3 = thresh[(n)];                                      \
	glow3  = glow[(n)];                                         \

/* *********************************************************** */

#define	DO_THRESH(s0, th, gl)                                          \
	(((s0) & (((th) - (mlib_s32) (s0)) >> 31)) | ((gl) & ~(((th) -   \
		(mlib_s32) (s0)) >> 31)))

#define	THRESH0(s0)	DO_THRESH(s0, thresh0, glow0)
#define	THRESH1(s0)	DO_THRESH(s0, thresh1, glow1)
#define	THRESH2(s0)	DO_THRESH(s0, thresh2, glow2)
#define	THRESH3(s0)	DO_THRESH(s0, thresh3, glow3)

/* *********************************************************** */

#define	SET_Pointers_M128(dst, src)                              \
	dst = (__m128i *) ((mlib_addr)pdst_row);                     \
	src = (__m128i *) ((mlib_addr)psrc_row);
/* *********************************************************** */

#define	DO_FAST_THRESH_U8(dst, src, th_SSE2, gl_SSE2)            \
	ss = _mm_loadu_si128(src);                                   \
	_m_u8x16 = _mm_xor_si128(ss, mask80);                        \
	mask_gt_th = _mm_cmpgt_epi8(_m_u8x16, th_SSE2);              \
	dd =                                                         \
		_mm_or_si128(                                            \
		_mm_andnot_si128(mask_gt_th, gl_SSE2),                   \
	    _mm_and_si128(ss, mask_gt_th));                          \
	_mm_storeu_si128(dst, dd);                                   \
	dst++;                                                       \
	src++

/* *********************************************************** */

#define	GROSS_LOOPS_U8                                           \
	for (j = 0; j < w16; j++) {                                  \
	    DO_FAST_THRESH_U8(dataPtr, sourcePtr, _m_th, _m_gl);     \
	}
/* *********************************************************** */


    void mlib_s_ImageThresh2_U81(
    const mlib_u8 *psrc,
    mlib_s32 src_stride,
    mlib_u8 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_u8 *thresh,
    const mlib_u8 *glow)
{
	mlib_s32 i, j, w16 = width >> 4;
	mlib_s32 thresh0 = thresh[0], glow0 = glow[0];
	const mlib_u8 *psrc_row = psrc;
	mlib_u8 *pdst_row = pdst;
	__m128i *sourcePtr, *dataPtr, ss, dd;
	__m128i _m_th = _mm_loadu_si128((__m128i *) thresh);
	__m128i _m_gl = _mm_loadu_si128((__m128i *) glow);
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
mlib_s_ImageThresh2_U82(
    const mlib_u8 *psrc,
    mlib_s32 src_stride,
    mlib_u8 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_u8 *thresh,
    const mlib_u8 *glow)
{
	mlib_s32 i, j, w16 = ((width <<= 1) >> 4);
	mlib_s32 thresh0 = thresh[0], thresh1 = thresh[1];
	mlib_s32 glow0 = glow[0], glow1 = glow[1];
	const mlib_u8 *psrc_row = psrc;
	mlib_u8 *pdst_row = pdst;

	__m128i *dataPtr, *sourcePtr, ss, dd;
	__m128i _m_th = _mm_loadu_si128((__m128i *) thresh);
	__m128i _m_gl = _mm_loadu_si128((__m128i *) glow);
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
mlib_s_ImageThresh2_U83(
    const mlib_u8 *psrc,
    mlib_s32 src_stride,
    mlib_u8 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_u8 *thresh,
    const mlib_u8 *glow)
{
	mlib_s32 i, j, w16 = ((width *= 3) >> 4);
	mlib_s32 thresh0 = thresh[0], thresh1 = thresh[1], thresh2 = thresh[2];
	mlib_s32 glow0 = glow[0], glow1 = glow[1], glow2 = glow[2];
	const mlib_u8 *psrc_row = psrc;
	mlib_u8 *pdst_row = pdst;

	__m128i *dataPtr, *sourcePtr, dd, ss;
	__m128i _m_th0 = _mm_loadu_si128(((__m128i *) thresh) + 0);
	__m128i _m_gl0 = _mm_loadu_si128(((__m128i *) glow) + 0);
	__m128i _m_th1 = _mm_loadu_si128(((__m128i *) thresh) + 1);
	__m128i _m_gl1 = _mm_loadu_si128(((__m128i *) glow) + 1);
	__m128i _m_th2 = _mm_loadu_si128(((__m128i *) thresh) + 2);
	__m128i _m_gl2 = _mm_loadu_si128(((__m128i *) glow) + 2);
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
			    _m_th0, _m_gl0);
			DO_FAST_THRESH_U8(dataPtr, sourcePtr,
			    _m_th1, _m_gl1);
			DO_FAST_THRESH_U8(dataPtr, sourcePtr,
			    _m_th2, _m_gl2);
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
mlib_s_ImageThresh2_U84(
    const mlib_u8 *psrc,
    mlib_s32 src_stride,
    mlib_u8 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_u8 *thresh,
    const mlib_u8 *glow)
{
	mlib_s32 i, j, w16 = ((width <<= 2) >> 4);
	mlib_s32 thresh0 = thresh[0], thresh1 = thresh[1], thresh2 =
	    thresh[2], thresh3 = thresh[3];
	mlib_s32 glow0 = glow[0], glow1 = glow[1], glow2 = glow[2], glow3 =
	    glow[3];
	const mlib_u8 *psrc_row = psrc;
	mlib_u8 *pdst_row = pdst;

	__m128i *dataPtr, *sourcePtr, ss, dd;
	__m128i _m_th = _mm_loadu_si128((__m128i *) thresh);
	__m128i _m_gl = _mm_loadu_si128((__m128i *) glow);
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
#define	DO_FAST_THRESH_S16(dst, src, th_SSE2, gl_SSE2)             \
	_m_16x8 = _mm_loadu_si128(src);                                \
	mask_gt_th = _mm_cmpgt_epi16(_m_16x8, th_SSE2);                \
	dd =                                                           \
		_mm_or_si128(                                              \
		_mm_andnot_si128(mask_gt_th, gl_SSE2),                     \
	    _mm_and_si128(_m_16x8, mask_gt_th));                       \
	_mm_storeu_si128(dst, dd);                                     \
	dst++;                                                         \
	src++


/* *********************************************************** */

#define	GROSS_LOOPS_S16                                            \
	for (j = 0; j < w7; j += 8) {                                  \
	    DO_FAST_THRESH_S16(dataPtr, sourcePtr, _m_th, _m_gl);      \
	}
/* *********************************************************** */

void
mlib_s_ImageThresh2_S161(
    const mlib_s16 *psrc,
    mlib_s32 src_stride,
    mlib_s16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_s32 *thresh,
    const mlib_s32 *glow)
{
	mlib_s32 i, j, k, w7 = width - 7;
	mlib_s32 thresh0 = thresh[0], glow0 = glow[0];
	const mlib_s16 *psrc_row = psrc;
	mlib_s16 *pdst_row = pdst;
	__m128i *dataPtr, *sourcePtr, dd;
	__m128i _m_th = _mm_set1_epi16(thresh[0]);
	__m128i _m_gl = _mm_set1_epi16(glow[0]);
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
mlib_s_ImageThresh2_S162(
    const mlib_s16 *psrc,
    mlib_s32 src_stride,
    mlib_s16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_s32 *thresh,
    const mlib_s32 *glow)
{
	mlib_s32 i, j, k, w7;
	mlib_s32 thresh0 = thresh[0], thresh1 = thresh[1];
	mlib_s32 glow0 = glow[0], glow1 = glow[1];
	const mlib_s16 *psrc_row = psrc;
	mlib_s16 *pdst_row = pdst;

	mlib_s32 th = (thresh1 << 16) | (thresh0 & 0xFFFF);
	mlib_s32 gl = (glow1 << 16) | (glow0 & 0xFFFF);

	__m128i *dataPtr, *sourcePtr, dd;
	__m128i _m_th = _mm_set1_epi32(th);
	__m128i _m_gl = _mm_set1_epi32(gl);
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
mlib_s_ImageThresh2_S163(
    const mlib_s16 *psrc,
    mlib_s32 src_stride,
    mlib_s16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_s32 *thresh,
    const mlib_s32 *glow)
{
	mlib_s32 i, j, k;
	mlib_s32 thresh0, thresh1, thresh2;
	mlib_s32 glow0, glow1, glow2;
	mlib_s32 ghigh0, ghigh1, ghigh2;
	const mlib_s16 *psrc_row = psrc;
	mlib_s16 *pdst_row = pdst;
	__m128i *dataPtr, *sourcePtr, dd;
	__m128i _m_th0, _m_th1, _m_th2;
	__m128i _m_gh0, _m_gh1, _m_gh2, _m_gl0, _m_gl1, _m_gl2;
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
		__m128i temp10_3 = _mm_cvtsi32_si128(
					(glow[0] & 0xFFFF) |
					(glow[1] << 16));
		__m128i temp02_3 = _mm_cvtsi32_si128(
					(glow[2] & 0xFFFF) |
					(glow[0] << 16));
		__m128i temp21_3 = _mm_cvtsi32_si128(
					(glow[1] & 0xFFFF) |
					(glow[2] << 16));

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
		_m_gl0 =
			_mm_or_si128(_mm_slli_si128(
			_mm_or_si128(_mm_slli_epi64(
			temp10_3, 32), temp21_3), 8),
			_mm_or_si128(_mm_slli_epi64(
			temp02_3, 32), temp10_3));
		_m_gl1 =
			_mm_or_si128(_mm_slli_si128(
			_mm_or_si128(_mm_slli_epi64(
			temp02_3, 32), temp10_3), 8),
			_mm_or_si128(_mm_slli_epi64(
			temp21_3, 32), temp02_3));
		_m_gl2 =
			_mm_or_si128(_mm_slli_si128(
			_mm_or_si128(_mm_slli_epi64(
			temp21_3, 32), temp02_3), 8),
			_mm_or_si128(_mm_slli_epi64(
			temp10_3, 32), temp21_3));
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
			    _m_th0, _m_gl0);
			DO_FAST_THRESH_S16(dataPtr, sourcePtr,
			    _m_th1, _m_gl1);
			DO_FAST_THRESH_S16(dataPtr, sourcePtr,
			    _m_th2, _m_gl2);
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
mlib_s_ImageThresh2_S164(
    const mlib_s16 *psrc,
    mlib_s32 src_stride,
    mlib_s16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_s32 *thresh,
    const mlib_s32 *glow)
{
	mlib_s32 i, j, k, w7;
	mlib_s32 thresh0 = thresh[0], thresh1 = thresh[1], thresh2 =
	    thresh[2], thresh3 = thresh[3];
	mlib_s32 glow0 = glow[0], glow1 = glow[1], glow2 = glow[2], glow3 =
	    glow[3];
	__m128i *dataPtr, *sourcePtr, dd;
	const mlib_s16 *psrc_row = psrc;
	mlib_s16 *pdst_row = pdst;

	__m128i _m_th =
	    _mm_or_si128(_mm_slli_epi64(
		_mm_cvtsi32_si128((thresh3 << 16) | (thresh2 & 0xffff)), 32),
		_mm_cvtsi32_si128((thresh1 << 16) | (thresh0 & 0xffff)));
	_m_th = _mm_unpacklo_epi64(_m_th, _m_th);

	__m128i _m_gl =
	    _mm_or_si128(_mm_slli_epi64(
		_mm_cvtsi32_si128((glow3 << 16) | (glow2 & 0xffff)), 32),
		_mm_cvtsi32_si128((glow1 << 16) | (glow0 & 0xffff)));
	_m_gl = _mm_unpacklo_epi64(_m_gl, _m_gl);

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
#define	DO_FAST_THRESH_U16(dst, src, th_SSE2, gl_SSE2)            \
	ss = _mm_loadu_si128(src);                                    \
	_m_16x8 = _mm_xor_si128(ss, mask8000);                        \
	mask_gt_th = _mm_cmpgt_epi16(_m_16x8, th_SSE2);               \
	dd =                                                          \
		_mm_or_si128(                                             \
		_mm_andnot_si128(mask_gt_th, gl_SSE2),                    \
	    _mm_and_si128(ss, mask_gt_th));                           \
	_mm_storeu_si128(dst, dd);                                    \
	dst++;                                                        \
	src++
/* *********************************************************** */

#define	GROSS_LOOPS_U16                                           \
	for (j = 0; j < w7; j += 8) {                                 \
	    DO_FAST_THRESH_U16(dataPtr, sourcePtr, _m_th, _m_gl);     \
	}
/* *********************************************************** */


void
mlib_s_ImageThresh2_U161(
    const mlib_u16 *psrc,
    mlib_s32 src_stride,
    mlib_u16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_s32 *thresh,
    const mlib_s32 *glow)
{
	mlib_s32 i, j, k, w7 = width - 7;
	mlib_s32 thresh0 = thresh[0], glow0 = glow[0];
	const mlib_u16 *psrc_row = psrc;
	mlib_u16 *pdst_row = pdst;
	__m128i *dataPtr, *sourcePtr, ss, dd;
	__m128i _m_th = _mm_set1_epi16(thresh[0]);
	__m128i _m_gl = _mm_set1_epi16(glow[0]);
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
mlib_s_ImageThresh2_U162(
    const mlib_u16 *psrc,
    mlib_s32 src_stride,
    mlib_u16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_s32 *thresh,
    const mlib_s32 *glow)
{
	mlib_s32 i, j, k, w7;
	mlib_s32 thresh0 = thresh[0], thresh1 = thresh[1];
	mlib_s32 glow0 = glow[0], glow1 = glow[1];
	const mlib_u16 *psrc_row = psrc;
	mlib_u16 *pdst_row = pdst;

	mlib_s32 th = (thresh1 << 16) | (thresh0 & 0xFFFF);
	mlib_s32 gl = (glow1 << 16) | (glow0 & 0xFFFF);

	__m128i *dataPtr, *sourcePtr, ss, dd;
	__m128i _m_th = _mm_set1_epi32(th);
	__m128i _m_gl = _mm_set1_epi32(gl);
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
mlib_s_ImageThresh2_U163(
    const mlib_u16 *psrc,
    mlib_s32 src_stride,
    mlib_u16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_s32 *thresh,
    const mlib_s32 *glow)
{
	mlib_s32 i, j, k;
	mlib_s32 thresh0, thresh1, thresh2;
	mlib_s32 glow0, glow1, glow2;
	mlib_s32 ghigh0, ghigh1, ghigh2;
	const mlib_u16 *psrc_row = psrc;
	mlib_u16 *pdst_row = pdst;
	__m128i *dataPtr, *sourcePtr, ss, dd;
	__m128i _m_th0, _m_th1, _m_th2;
	__m128i _m_gl0, _m_gl1, _m_gl2;
	__m128i _m_16x8, mask_gt_th;
	__m128i mask8000 = _mm_set1_epi32((int)0x80008000);

	{
		mlib_s32 temp10_0 = (thresh[0] & 0xFFFF) | (thresh[1] << 16);
		mlib_s32 temp02_0 = (thresh[2] & 0xFFFF) | (thresh[0] << 16);
		mlib_s32 temp21_0 = (thresh[1] & 0xFFFF) | (thresh[2] << 16);
		mlib_s32 temp10_3 = (glow[0] & 0xFFFF) | (glow[1] << 16);
		mlib_s32 temp02_3 = (glow[2] & 0xFFFF) | (glow[0] << 16);
		mlib_s32 temp21_3 = (glow[1] & 0xFFFF) | (glow[2] << 16);

		_m_th0 = _mm_setr_epi32(temp10_0, temp02_0, temp21_0, temp10_0);
		_m_th1 = _mm_setr_epi32(temp02_0, temp21_0, temp10_0, temp02_0);
		_m_th2 = _mm_setr_epi32(temp21_0, temp10_0, temp02_0, temp21_0);
		_m_gl0 = _mm_setr_epi32(temp10_3, temp02_3, temp21_3, temp10_3);
		_m_gl1 = _mm_setr_epi32(temp02_3, temp21_3, temp10_3, temp02_3);
		_m_gl2 = _mm_setr_epi32(temp21_3, temp10_3, temp02_3, temp21_3);
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
			    _m_th0, _m_gl0);
			DO_FAST_THRESH_U16(dataPtr, sourcePtr,
			    _m_th1, _m_gl1);
			DO_FAST_THRESH_U16(dataPtr, sourcePtr,
			    _m_th2, _m_gl2);
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
mlib_s_ImageThresh2_U164(
    const mlib_u16 *psrc,
    mlib_s32 src_stride,
    mlib_u16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_s32 *thresh,
    const mlib_s32 *glow)
{
	mlib_s32 i, j, k, w7;
	mlib_s32 thresh0 = thresh[0], thresh1 = thresh[1], thresh2 =
	    thresh[2], thresh3 = thresh[3];
	mlib_s32 glow0 = glow[0], glow1 = glow[1], glow2 = glow[2], glow3 =
	    glow[3];
	__m128i *dataPtr, *sourcePtr, ss, dd;
	const mlib_u16 *psrc_row = psrc;
	mlib_u16 *pdst_row = pdst;

	__m128i _m_th =
	    _mm_or_si128(_mm_slli_epi64(
		_mm_cvtsi32_si128((thresh3 << 16) | (thresh2 & 0xffff)), 32),
		_mm_cvtsi32_si128((thresh1 << 16) | (thresh0 & 0xffff)));
	_m_th = _mm_unpacklo_epi64(_m_th, _m_th);

	__m128i _m_gl =
	    _mm_or_si128(_mm_slli_epi64(
		_mm_cvtsi32_si128((glow3 << 16) | (glow2 & 0xffff)), 32),
		_mm_cvtsi32_si128((glow1 << 16) | (glow0 & 0xffff)));
	_m_gl = _mm_unpacklo_epi64(_m_gl, _m_gl);

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

#define	DO_THRESH_S32(s0, th, gl)                         \
	((s0) > (th)) ? (s0) : (gl)

#define	THRESH0_S32(s0)	DO_THRESH_S32(s0, thresh0, glow0)
#define	THRESH1_S32(s0)	DO_THRESH_S32(s0, thresh1, glow1)
#define	THRESH2_S32(s0)	DO_THRESH_S32(s0, thresh2, glow2)
#define	THRESH3_S32(s0)	DO_THRESH_S32(s0, thresh3, glow3)

/* *********************************************************** */
#define	DO_FAST_THRESH_S32(dst, src, th_SSE2, gl_SSE2)             \
	_m_32x4 = _mm_loadu_si128(src);                                \
	mask_gt_th = _mm_cmpgt_epi32(_m_32x4, th_SSE2);                \
	dd =                                                           \
		_mm_or_si128(                                              \
		_mm_andnot_si128(mask_gt_th, gl_SSE2),                     \
	    _mm_and_si128(_m_32x4, mask_gt_th));                       \
	_mm_storeu_si128(dst, dd);                                     \
	dst++;                                                         \
	src++
/* *********************************************************** */

#define	GROSS_LOOPS_S32                                            \
	for (j = 0; j < w3; j += 4) {                                  \
	    DO_FAST_THRESH_S32(dataPtr, sourcePtr, _m_th, _m_gl);      \
	}
/* *********************************************************** */


void
mlib_s_ImageThresh2_S321(
    const mlib_s32 *psrc,
    mlib_s32 src_stride,
    mlib_s32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_s32 *thresh,
    const mlib_s32 *glow)
{
	mlib_s32 i, j, k, w3 = width - 3;
	const mlib_s32 *psrc_row = psrc;
	mlib_s32 thresh0, ghigh0, glow0;
	mlib_s32 *pdst_row = pdst;
	__m128i *dataPtr, *sourcePtr, dd;
	__m128i _m_th = _mm_set1_epi32(thresh[0]);
	__m128i _m_gl = _mm_set1_epi32(glow[0]);
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
mlib_s_ImageThresh2_S322(
    const mlib_s32 *psrc,
    mlib_s32 src_stride,
    mlib_s32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_s32 *thresh,
    const mlib_s32 *glow)
{
	mlib_s32 i, j, k, w3 = width;
	const mlib_s32 *psrc_row = psrc;
	mlib_s32 *pdst_row = pdst;
	mlib_s32 thresh0, thresh1;
	mlib_s32 ghigh0, ghigh1;
	mlib_s32 glow0, glow1;

	__m128i *dataPtr, *sourcePtr, dd;
	__m128i _m_th = _mm_setr_epi32(
			thresh[0], thresh[1], thresh[0], thresh[1]);
	__m128i _m_gl = _mm_setr_epi32(
			glow[0], glow[1], glow[0], glow[1]);
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
mlib_s_ImageThresh2_S323(
    const mlib_s32 *psrc,
    mlib_s32 src_stride,
    mlib_s32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_s32 *thresh,
    const mlib_s32 *glow)
{
	mlib_s32 i, j, k;
	mlib_s32 thresh0, thresh1, thresh2;
	mlib_s32 glow0, glow1, glow2;
	const mlib_s32 *psrc_row = psrc;
	mlib_s32 *pdst_row = pdst;
	__m128i *dataPtr, *sourcePtr, dd;
	__m128i _m_th0 = _mm_setr_epi32(thresh[0], thresh[1],
						thresh[2], thresh[0]);
	__m128i _m_th1 = _mm_setr_epi32(thresh[1], thresh[2],
						thresh[0], thresh[1]);
	__m128i _m_th2 = _mm_setr_epi32(thresh[2], thresh[0],
						thresh[1], thresh[2]);
	__m128i _m_gl0 = _mm_setr_epi32(glow[0], glow[1],
						glow[2], glow[0]);
	__m128i _m_gl1 = _mm_setr_epi32(glow[1], glow[2],
						glow[0], glow[1]);
	__m128i _m_gl2 = _mm_setr_epi32(glow[2], glow[0],
						glow[1], glow[2]);
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
			    _m_th0, _m_gl0);
			DO_FAST_THRESH_S32(dataPtr, sourcePtr,
			    _m_th1, _m_gl1);
			DO_FAST_THRESH_S32(dataPtr, sourcePtr,
			    _m_th2, _m_gl2);
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
mlib_s_ImageThresh2_S324(
    const mlib_s32 *psrc,
    mlib_s32 src_stride,
    mlib_s32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_s32 *thresh,
    const mlib_s32 *glow)
{
	mlib_s32 i, j, k, w3;
	const mlib_s32 *psrc_row = psrc;
	mlib_s32 *pdst_row = pdst;

	__m128i *dataPtr, *sourcePtr, dd;
	__m128i _m_th = _mm_setr_epi32(
			thresh[0], thresh[1], thresh[2], thresh[3]);
	__m128i _m_gl = _mm_setr_epi32(
			glow[0], glow[1], glow[2], glow[3]);
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
__mlib_ImageThresh2(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *thresh,
    const mlib_s32 *glow)
{
	mlib_s32 sstride, dstride, width, height, nchan;
	mlib_type dtype, stype;
	void *psrc, *pdst;
	mlib_s32 th[4], gl[4];
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
	if (glow == NULL)
		return (MLIB_NULLPOINTER);

	if (dtype == MLIB_BYTE) {
		mlib_u8 th_u8[48], gl_u8[48];

		for (i = 0; i < nchan; i++) {
			gl[i] = glow[i] & 255;
			th[i] = thresh[i];
			if (thresh[i] < 0) {
				gl[i] = 0;
				th[i] = 0;
			} else if (thresh[i] > MLIB_U8_MAX) {
				th[i] = MLIB_U8_MAX;
			}
		}

		for (j = 0; j < 48; j += i) {
			for (i = 0; i < nchan; i++) {
				gl_u8[j + i] = (mlib_u8)gl[i];
				th_u8[j + i] = (mlib_u8)th[i];
			}
		}

		switch (nchan) {

		case 1:
			mlib_s_ImageThresh2_U81((mlib_u8 *)psrc,
				sstride, (mlib_u8 *)pdst, dstride,
				width, height, th_u8, gl_u8);
			break;
		case 2:
			mlib_s_ImageThresh2_U82((mlib_u8 *)psrc,
				sstride, (mlib_u8 *)pdst, dstride,
				width, height, th_u8, gl_u8);
			break;
		case 3:
			mlib_s_ImageThresh2_U83((mlib_u8 *)psrc,
				sstride, (mlib_u8 *)pdst, dstride,
				width, height, th_u8, gl_u8);
			break;
		case 4:
			mlib_s_ImageThresh2_U84((mlib_u8 *)psrc,
				sstride, (mlib_u8 *)pdst, dstride,
				width, height, th_u8, gl_u8);
			break;
		}
		return (MLIB_SUCCESS);

	} else if (dtype == MLIB_SHORT) {

		for (i = 0; i < nchan; i++) {
			gl[i] = (glow[i] << 16) >> 16;
			th[i] = thresh[i];
			if (thresh[i] > MLIB_S16_MAX)
				th[i] = MLIB_S16_MAX;
			else if (thresh[i] < MLIB_S16_MIN) {
				th[i] = MLIB_S16_MIN;
				gl[i] = MLIB_S16_MIN;
			}
		}

		switch (nchan) {
			case 1:
			mlib_s_ImageThresh2_S161((mlib_s16 *)psrc,
				sstride, (mlib_s16 *)pdst, dstride,
				width, height, th, gl);
			break;
		case 2:
			mlib_s_ImageThresh2_S162((mlib_s16 *)psrc,
				sstride, (mlib_s16 *)pdst, dstride,
				width, height, th, gl);
			break;
		case 3:
			mlib_s_ImageThresh2_S163((mlib_s16 *)psrc,
				sstride, (mlib_s16 *)pdst, dstride,
				width, height, th, gl);
			break;
		case 4:
			mlib_s_ImageThresh2_S164((mlib_s16 *)psrc,
				sstride, (mlib_s16 *)pdst, dstride,
				width, height, th, gl);
			break;
		}
		return (MLIB_SUCCESS);

	} else if (dtype == MLIB_USHORT) {

		for (i = 0; i < nchan; i++) {
			gl[i] = (glow[i] << 16) >> 16;
			th[i] = thresh[i];
			if (thresh[i] > MLIB_U16_MAX) {
				th[i] = MLIB_U16_MAX;
			} if (thresh[i] < MLIB_U16_MIN) {
				th[i] = MLIB_U16_MIN;
				gl[i] = MLIB_U16_MIN;
			}
		}

		switch (nchan) {
		case 1:
			mlib_s_ImageThresh2_U161(
				psrc, sstride, pdst, dstride,
				width, height, th, gl);
			break;
		case 2:
			mlib_s_ImageThresh2_U162(
				psrc, sstride, pdst, dstride,
				width, height, th, gl);
			break;
		case 3:
			mlib_s_ImageThresh2_U163(
				psrc, sstride, pdst, dstride,
				width, height, th, gl);
			break;
		case 4:
			mlib_s_ImageThresh2_U164(
				psrc, sstride, pdst, dstride,
				width, height, th, gl);
			break;
		}
		return (MLIB_SUCCESS);

	} else if (dtype == MLIB_INT) {

		switch (nchan) {
		case 1:
			mlib_s_ImageThresh2_S321(
				(mlib_s32 *)psrc, sstride,
			    (mlib_s32 *)pdst, dstride,
				width, height, thresh, glow);
			break;
		case 2:
			mlib_s_ImageThresh2_S322(
				(mlib_s32 *)psrc, sstride,
			    (mlib_s32 *)pdst, dstride,
				width, height, thresh, glow);
			break;
		case 3:
			mlib_s_ImageThresh2_S323(
				(mlib_s32 *)psrc, sstride,
			    (mlib_s32 *)pdst, dstride,
				width, height, thresh, glow);
			break;
		case 4:
			mlib_s_ImageThresh2_S324(
				(mlib_s32 *)psrc, sstride,
			    (mlib_s32 *)pdst, dstride,
				width, height, thresh, glow);
			break;
		}
		return (MLIB_SUCCESS);
	}

	return (MLIB_FAILURE);
}
/* *********************************************************** */
