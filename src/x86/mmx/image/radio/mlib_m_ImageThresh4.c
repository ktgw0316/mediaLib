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

#pragma ident	"@(#)mlib_m_ImageThresh4.c	9.6	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageThresh4 - thresholding
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageThresh4(mlib_image *dst,
 *                                    const mlib_image *src,
 *                                    const mlib_s32   *thigh,
 *                                    const mlib_s32   *tlow,
 *                                    const mlib_s32   *ghigh,
 *                                    const mlib_s32   *glow);
 *
 * ARGUMENT
 *      dst     pointer to output image
 *      src     pointer to input image
 *      thigh   array of high thresholds
 *      tlow    array of low thresholds
 *      ghigh   array of values above thresholds
 *      glow    array of values below thresholds
 *
 * RESTRICTION
 *      The images must have the same type, the same size, and the same number
 *      of channels.
 *      The images can have 1, 2, 3, or 4 channels.
 *      The images can be in MLIB_BYTE, MLIB_SHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *      If the pixel channel value is above the thigh for that channel,
 *      set the destination to the ghigh value for that channel.
 *      If the pixel channel value is below the tlow for that channel,
 *      set the destination to the glow value for that channel.
 *
 *                      +- glow[c]      src[x][y][c] < tlow[c]
 *                      |
 *      dst[x][y][c]  = +- src[x][y][c] tlow[c] <= src[x][y][c] <= thigh[c]
 *                      |
 *                      +- ghigh[c]     src[x][y][c] > thigh[c]
 */

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <mmintrin.h>
#endif

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_mmx_utils.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageThresh4 = __mlib_ImageThresh4

#elif defined(__GNUC__)  /* defined(__SUNPRO_C) */

__typeof__(__mlib_ImageThresh4) mlib_ImageThresh4
    __attribute__((weak, alias("__mlib_ImageThresh4")));

#else /* defined(__SUNPRO_C) */

#error "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

    void mlib_m_ImageThresh4_U81(
    const mlib_u8 *psrc,
    mlib_s32 src_stride,
    mlib_u8 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_u8 *thigh,
    const mlib_u8 *tlow,
    const mlib_u8 *ghigh,
    const mlib_u8 *glow);
    void mlib_m_ImageThresh4_U82(
    const mlib_u8 *psrc,
    mlib_s32 src_stride,
    mlib_u8 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_u8 *thigh,
    const mlib_u8 *tlow,
    const mlib_u8 *ghigh,
    const mlib_u8 *glow);
    void mlib_m_ImageThresh4_U83(
    const mlib_u8 *psrc,
    mlib_s32 src_stride,
    mlib_u8 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_u8 *thigh,
    const mlib_u8 *tlow,
    const mlib_u8 *ghigh,
    const mlib_u8 *glow);
    void mlib_m_ImageThresh4_U84(
    const mlib_u8 *psrc,
    mlib_s32 src_stride,
    mlib_u8 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_u8 *thigh,
    const mlib_u8 *tlow,
    const mlib_u8 *ghigh,
    const mlib_u8 *glow);

    void mlib_m_ImageThresh4_S161(
    const mlib_s16 *psrc,
    mlib_s32 src_stride,
    mlib_s16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *ghigh,
    const mlib_s32 *glow);
    void mlib_m_ImageThresh4_S162(
    const mlib_s16 *psrc,
    mlib_s32 src_stride,
    mlib_s16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *ghigh,
    const mlib_s32 *glow);
    void mlib_m_ImageThresh4_S163(
    const mlib_s16 *psrc,
    mlib_s32 src_stride,
    mlib_s16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *ghigh,
    const mlib_s32 *glow);
    void mlib_m_ImageThresh4_S164(
    const mlib_s16 *psrc,
    mlib_s32 src_stride,
    mlib_s16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *ghigh,
    const mlib_s32 *glow);

    void mlib_m_ImageThresh4_S321(
    const mlib_s32 *psrc,
    mlib_s32 src_stride,
    mlib_s32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *ghigh,
    const mlib_s32 *glow);
    void mlib_m_ImageThresh4_S322(
    const mlib_s32 *psrc,
    mlib_s32 src_stride,
    mlib_s32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *ghigh,
    const mlib_s32 *glow);
    void mlib_m_ImageThresh4_S323(
    const mlib_s32 *psrc,
    mlib_s32 src_stride,
    mlib_s32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *ghigh,
    const mlib_s32 *glow);
    void mlib_m_ImageThresh4_S324(
    const mlib_s32 *psrc,
    mlib_s32 src_stride,
    mlib_s32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *ghigh,
    const mlib_s32 *glow);

/* *********************************************************** */

#define	INIT_THRESH0(n)                                         \
	thigh0 = thigh[(n)];                                    \
	tlow0 = tlow[(n)];                                      \
	ghigh0 = ghigh[(n)];                                    \
	glow0 = glow[(n)];

#define	INIT_THRESH1(n)               \
	thigh1 = thigh[(n)];          \
	tlow1  = tlow[(n)];           \
	ghigh1 = ghigh[(n)];          \
	glow1  = glow[(n)];           \

#define	INIT_THRESH2(n)                                         \
	thigh2 = thigh[(n)];                                    \
	tlow2 = tlow[(n)];                                      \
	ghigh2 = ghigh[(n)];                                    \
	glow2 = glow[(n)];

#define	INIT_THRESH3(n)               \
	thigh3 = thigh[(n)];          \
	tlow3  = tlow[(n)];           \
	ghigh3 = ghigh[(n)];          \
	glow3  = glow[(n)];           \

/* *********************************************************** */

#define	DO_THRESH(s0, th, tl, gh, gl)                                 \
	((((s0 - tl) >> 31) & gl) | (((th -                           \
	    s0) >> 31) & gh) | ((s0 & ~((th - s0) >> 31)) & ~((s0 -   \
	    tl) >> 31)))

#define	THRESH0(s0)	DO_THRESH(s0, thigh0, tlow0, ghigh0, glow0)
#define	THRESH1(s0)	DO_THRESH(s0, thigh1, tlow1, ghigh1, glow1)
#define	THRESH2(s0)	DO_THRESH(s0, thigh2, tlow2, ghigh2, glow2)
#define	THRESH3(s0)	DO_THRESH(s0, thigh3, tlow3, ghigh3, glow3)

/* *********************************************************** */

void
mlib_m_ImageThresh4_U81(
    const mlib_u8 *psrc,
    mlib_s32 src_stride,
    mlib_u8 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_u8 *thigh,
    const mlib_u8 *tlow,
    const mlib_u8 *ghigh,
    const mlib_u8 *glow)
{
	mlib_s32 i, j, w8 = width >> 3;
	mlib_s32 thigh0 = thigh[0], tlow0 = tlow[0], ghigh0 = ghigh[0], glow0 =
	    glow[0];
	const mlib_u8 *psrc_row = psrc;
	mlib_u8 *pdst_row = pdst;

	__m64 *sourcePtr, *dataPtr, mask;
	__m64 _m_th = *((__m64 *) thigh);
	__m64 _m_tl = *((__m64 *) tlow);
	__m64 _m_gh = *((__m64 *) ghigh);
	__m64 _m_gl = *((__m64 *) glow);
	__m64 _m_u8x4, mask_gt_th, mask_ls_tl, u8_zero, s;
	__m64 mask80 = mmx_from_int_dup(0x80808080);

	_m_th = _m_pxor(_m_th, mask80);
	_m_tl = _m_pxor(_m_tl, mask80);

	for (i = 0; i < height; i++) {

		dataPtr = (__m64 *) ((mlib_addr)pdst_row);
		sourcePtr = (__m64 *) ((mlib_addr)psrc_row);

		for (j = 0; j < w8; j++) {
			s = sourcePtr[j + 0];
			_m_u8x4 = _m_pxor(s, mask80);
			mask_gt_th = _mm_cmpgt_pi8(_m_u8x4, _m_th);
			mask_ls_tl = _mm_cmpgt_pi8(_m_tl, _m_u8x4);
			mask = _m_por(mask_gt_th, mask_ls_tl);
			u8_zero = _m_pandn(mask, s);
			dataPtr[j + 0] =
			    _m_por(_m_por(_m_pand(mask_ls_tl, _m_gl),
			    _m_pand(mask_gt_th, _m_gh)), u8_zero);
		}

		for (j = (w8 << 3); j < width; j++)
			pdst_row[j] = THRESH0(psrc_row[j]);

		psrc_row += src_stride;
		pdst_row += dst_stride;
	}
	_m_empty();
}

/* *********************************************************** */

void
mlib_m_ImageThresh4_U82(
    const mlib_u8 *psrc,
    mlib_s32 src_stride,
    mlib_u8 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_u8 *thigh,
    const mlib_u8 *tlow,
    const mlib_u8 *ghigh,
    const mlib_u8 *glow)
{
	mlib_s32 i, j, w8 = ((width <<= 1) >> 3);
	mlib_s32 thigh0 = thigh[0], thigh1 = thigh[1];
	mlib_s32 tlow0 = tlow[0], tlow1 = tlow[1];
	mlib_s32 ghigh0 = ghigh[0], ghigh1 = ghigh[1];
	mlib_s32 glow0 = glow[0], glow1 = glow[1];
	const mlib_u8 *psrc_row = psrc;
	mlib_u8 *pdst_row = pdst;

	__m64 *dataPtr, *sourcePtr;

	__m64 _m_th = *((__m64 *) thigh);
	__m64 _m_tl = *((__m64 *) tlow);
	__m64 _m_gh = *((__m64 *) ghigh);
	__m64 _m_gl = *((__m64 *) glow);
	__m64 _m_u8x4, mask_gt_th, mask_ls_tl, mask, u8_zero, s;
	__m64 mask80 = mmx_from_int_dup(0x80808080);

	_m_th = _m_pxor(_m_th, mask80);
	_m_tl = _m_pxor(_m_tl, mask80);

	for (i = 0; i < height; i++) {

		dataPtr = (__m64 *) ((mlib_addr)pdst_row);
		sourcePtr = (__m64 *) ((mlib_addr)psrc_row);

		for (j = 0; j < w8; j++) {
			s = sourcePtr[j + 0];
			_m_u8x4 = _m_pxor(s, mask80);
			mask_gt_th = _mm_cmpgt_pi8(_m_u8x4, _m_th);
			mask_ls_tl = _mm_cmpgt_pi8(_m_tl, _m_u8x4);
			mask = _m_por(mask_gt_th, mask_ls_tl);
			u8_zero = _m_pandn(mask, s);
			dataPtr[j + 0] =
			    _m_por(_m_por(_m_pand(mask_ls_tl, _m_gl),
			    _m_pand(mask_gt_th, _m_gh)), u8_zero);
		}

		for (j = w8 << 3; j < width; j += 2) {
			pdst_row[j + 0] = THRESH0(psrc_row[j]);
			pdst_row[j + 1] = THRESH1(psrc_row[j + 1]);
		}

		psrc_row += src_stride;
		pdst_row += dst_stride;
	}
	_m_empty();
}

/* *********************************************************** */

void
mlib_m_ImageThresh4_U83(
    const mlib_u8 *psrc,
    mlib_s32 src_stride,
    mlib_u8 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_u8 *thigh,
    const mlib_u8 *tlow,
    const mlib_u8 *ghigh,
    const mlib_u8 *glow)
{
	mlib_s32 i, j, w8 = ((width *= 3) >> 3);
	mlib_s32 thigh0 = thigh[0], thigh1 = thigh[1], thigh2 = thigh[2];
	mlib_s32 tlow0 = tlow[0], tlow1 = tlow[1], tlow2 = tlow[2];
	mlib_s32 ghigh0 = ghigh[0], ghigh1 = ghigh[1], ghigh2 = ghigh[2];
	mlib_s32 glow0 = glow[0], glow1 = glow[1], glow2 = glow[2];
	const mlib_u8 *psrc_row = psrc;
	mlib_u8 *pdst_row = pdst;

	__m64 *dataPtr, *sourcePtr;
	__m64 _m_th0 = ((__m64 *) thigh)[0];
	__m64 _m_tl0 = ((__m64 *) tlow)[0];
	__m64 _m_gh0 = ((__m64 *) ghigh)[0];
	__m64 _m_gl0 = ((__m64 *) glow)[0];
	__m64 _m_th1 = ((__m64 *) thigh)[1];
	__m64 _m_tl1 = ((__m64 *) tlow)[1];
	__m64 _m_gh1 = ((__m64 *) ghigh)[1];
	__m64 _m_gl1 = ((__m64 *) glow)[1];
	__m64 _m_th2 = ((__m64 *) thigh)[2];
	__m64 _m_tl2 = ((__m64 *) tlow)[2];
	__m64 _m_gh2 = ((__m64 *) ghigh)[2];
	__m64 _m_gl2 = ((__m64 *) glow)[2];

	__m64 _m_u8x4_0, mask_gt_th_0, mask_ls_tl_0, mask_0, u8_zero_0, s_0;
	__m64 _m_u8x4_1, mask_gt_th_1, mask_ls_tl_1, mask_1, u8_zero_1, s_1;
	__m64 _m_u8x4_2, mask_gt_th_2, mask_ls_tl_2, mask_2, u8_zero_2, s_2;

	__m64 mask80 = mmx_from_int_dup(0x80808080);

	_m_th0 = _m_pxor(_m_th0, mask80);
	_m_tl0 = _m_pxor(_m_tl0, mask80);
	_m_th1 = _m_pxor(_m_th1, mask80);
	_m_tl1 = _m_pxor(_m_tl1, mask80);
	_m_th2 = _m_pxor(_m_th2, mask80);
	_m_tl2 = _m_pxor(_m_tl2, mask80);

	for (i = 0; i < height; i++) {

		dataPtr = (__m64 *) ((mlib_addr)pdst_row);
		sourcePtr = (__m64 *) ((mlib_addr)psrc_row);

		for (j = 0; j < (w8 - 2); j += 3) {
			s_0 = sourcePtr[j + 0];
			s_1 = sourcePtr[j + 1];
			s_2 = sourcePtr[j + 2];
			_m_u8x4_0 = _m_pxor(s_0, mask80);
			_m_u8x4_1 = _m_pxor(s_1, mask80);
			_m_u8x4_2 = _m_pxor(s_2, mask80);
			mask_gt_th_0 = _mm_cmpgt_pi8(_m_u8x4_0, _m_th0);
			mask_gt_th_1 = _mm_cmpgt_pi8(_m_u8x4_1, _m_th1);
			mask_gt_th_2 = _mm_cmpgt_pi8(_m_u8x4_2, _m_th2);
			mask_ls_tl_0 = _mm_cmpgt_pi8(_m_tl0, _m_u8x4_0);
			mask_ls_tl_1 = _mm_cmpgt_pi8(_m_tl1, _m_u8x4_1);
			mask_ls_tl_2 = _mm_cmpgt_pi8(_m_tl2, _m_u8x4_2);
			mask_0 = _m_por(mask_gt_th_0, mask_ls_tl_0);
			mask_1 = _m_por(mask_gt_th_1, mask_ls_tl_1);
			mask_2 = _m_por(mask_gt_th_2, mask_ls_tl_2);
			u8_zero_0 = _m_pandn(mask_0, s_0);
			u8_zero_1 = _m_pandn(mask_1, s_1);
			u8_zero_2 = _m_pandn(mask_2, s_2);
			dataPtr[j + 0] =
			    _m_por(_m_por(_m_pand(mask_ls_tl_0, _m_gl0),
			    _m_pand(mask_gt_th_0, _m_gh0)), u8_zero_0);
			dataPtr[j + 1] =
			    _m_por(_m_por(_m_pand(mask_ls_tl_1, _m_gl1),
			    _m_pand(mask_gt_th_1, _m_gh1)), u8_zero_1);
			dataPtr[j + 2] =
			    _m_por(_m_por(_m_pand(mask_ls_tl_2, _m_gl2),
			    _m_pand(mask_gt_th_2, _m_gh2)), u8_zero_2);
		}

		for (j <<= 3; j < width; j += 3) {
			pdst_row[j + 0] = THRESH0(psrc_row[j + 0]);
			pdst_row[j + 1] = THRESH1(psrc_row[j + 1]);
			pdst_row[j + 2] = THRESH2(psrc_row[j + 2]);
		}

		psrc_row += src_stride, pdst_row += dst_stride;
	}
	_m_empty();
}

/* *********************************************************** */

void
mlib_m_ImageThresh4_U84(
    const mlib_u8 *psrc,
    mlib_s32 src_stride,
    mlib_u8 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_u8 *thigh,
    const mlib_u8 *tlow,
    const mlib_u8 *ghigh,
    const mlib_u8 *glow)
{
	mlib_s32 i, j, w8 = ((width <<= 2) >> 3);
	mlib_s32 thigh0 = thigh[0], thigh1 = thigh[1], thigh2 =
	    thigh[2], thigh3 = thigh[3];
	mlib_s32 tlow0 = tlow[0], tlow1 = tlow[1], tlow2 = tlow[2], tlow3 =
	    tlow[3];
	mlib_s32 ghigh0 = ghigh[0], ghigh1 = ghigh[1], ghigh2 =
	    ghigh[2], ghigh3 = ghigh[3];
	mlib_s32 glow0 = glow[0], glow1 = glow[1], glow2 = glow[2], glow3 =
	    glow[3];
	const mlib_u8 *psrc_row = psrc;
	mlib_u8 *pdst_row = pdst;

	__m64 *dataPtr, *sourcePtr;
	__m64 _m_th = *((__m64 *) thigh);
	__m64 _m_tl = *((__m64 *) tlow);
	__m64 _m_gh = *((__m64 *) ghigh);
	__m64 _m_gl = *((__m64 *) glow);
	__m64 _m_u8x4, mask_gt_th, mask_ls_tl, mask, u8_zero, s;
	__m64 mask80 = mmx_from_int_dup(0x80808080);

	_m_th = _m_pxor(_m_th, mask80);
	_m_tl = _m_pxor(_m_tl, mask80);

	for (i = 0; i < height; i++) {

		dataPtr = (__m64 *) ((mlib_addr)pdst_row);
		sourcePtr = (__m64 *) ((mlib_addr)psrc_row);

		for (j = 0; j < w8; j++) {
			s = sourcePtr[j + 0];
			_m_u8x4 = _m_pxor(s, mask80);
			mask_gt_th = _mm_cmpgt_pi8(_m_u8x4, _m_th);
			mask_ls_tl = _mm_cmpgt_pi8(_m_tl, _m_u8x4);
			mask = _m_por(mask_gt_th, mask_ls_tl);
			u8_zero = _m_pandn(mask, s);
			dataPtr[j + 0] =
			    _m_por(_m_por(_m_pand(mask_ls_tl, _m_gl),
			    _m_pand(mask_gt_th, _m_gh)), u8_zero);
		}

		if ((j <<= 3) < width) {
			pdst_row[j + 0] = THRESH0(psrc_row[j + 0]);
			pdst_row[j + 1] = THRESH1(psrc_row[j + 1]);
			pdst_row[j + 2] = THRESH2(psrc_row[j + 2]);
			pdst_row[j + 3] = THRESH3(psrc_row[j + 3]);
		}

		psrc_row += src_stride, pdst_row += dst_stride;
	}
	_m_empty();
}

/* *********************************************************** */

#define	SET_Pointers_M64(dst, src)                              \
	dst = (__m64 *) (pdst_row);                             \
	src = (__m64 *) (psrc_row);

/* *********************************************************** */

#define	DO_FAST_THRESH_S16(dst, src, th_MMX, tl_MMX, gh_MMX,        \
	gl_MMX)                                                     \
	_m_16x4 = src;                                              \
	mask_gt_th = _mm_cmpgt_pi16(_m_16x4, th_MMX);               \
	mask_ls_tl = _mm_cmpgt_pi16(tl_MMX, _m_16x4);               \
	dst =                                                       \
	_m_por(_m_pandn(_m_por(mask_gt_th, mask_ls_tl), _m_16x4),   \
	    _m_por(_m_pand(gl_MMX, mask_ls_tl), _m_pand(gh_MMX,     \
	    mask_gt_th)));

/* *********************************************************** */

#define	GROSS_LOOPS_S16                                                  \
	for (j = 0, k = 0; j < w3; j += 4, k++) {                        \
	    DO_FAST_THRESH_S16(dataPtr[k], sourcePtr[k], _m_th, _m_tl,   \
		_m_gh, _m_gl);                                           \
	}

/* *********************************************************** */

void
mlib_m_ImageThresh4_S161(
    const mlib_s16 *psrc,
    mlib_s32 src_stride,
    mlib_s16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *ghigh,
    const mlib_s32 *glow)
{
	mlib_s32 i, j, k, w3 = width - 3;
	mlib_s32 thigh0 = thigh[0],
	    tlow0 = tlow[0], ghigh0 = ghigh[0], glow0 = glow[0];
	const mlib_s16 *psrc_row = psrc;
	mlib_s16 *pdst_row = pdst;
	__m64 *dataPtr, *sourcePtr;
	__m64 _m_th = _mm_set1_pi16(thigh[0]);
	__m64 _m_tl = _mm_set1_pi16(tlow[0]);
	__m64 _m_gh = _mm_set1_pi16(ghigh[0]);
	__m64 _m_gl = _mm_set1_pi16(glow[0]);
	__m64 _m_16x4, mask_gt_th, mask_ls_tl;

	for (i = 0; i < height; i++) {

		SET_Pointers_M64(dataPtr, sourcePtr);

		GROSS_LOOPS_S16;

		for (; j < width; j++)
			pdst_row[j] = THRESH0(psrc_row[j]);

		psrc_row += (src_stride >> 1);
		pdst_row += (dst_stride >> 1);
	}
	_m_empty();
}

/* *********************************************************** */

void
mlib_m_ImageThresh4_S162(
    const mlib_s16 *psrc,
    mlib_s32 src_stride,
    mlib_s16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *ghigh,
    const mlib_s32 *glow)
{
	mlib_s32 i, j, k, w3;
	mlib_s32 thigh0 = thigh[0], thigh1 = thigh[1];
	mlib_s32 tlow0 = tlow[0], tlow1 = tlow[1];
	mlib_s32 ghigh0 = ghigh[0], ghigh1 = ghigh[1];
	mlib_s32 glow0 = glow[0], glow1 = glow[1];
	const mlib_s16 *psrc_row = psrc;
	mlib_s16 *pdst_row = pdst;
	__m64 *dataPtr, *sourcePtr;

	mlib_s32 th = (thigh1 << 16) | (thigh0 & 0xFFFF);
	mlib_s32 tl = (tlow1 << 16) | (tlow0 & 0xFFFF);
	mlib_s32 gh = (ghigh1 << 16) | (ghigh0 & 0xFFFF);
	mlib_s32 gl = (glow1 << 16) | (glow0 & 0xFFFF);

	__m64 _m_th = _mm_setr_pi32(th, th);
	__m64 _m_tl = _mm_setr_pi32(tl, tl);
	__m64 _m_gh = _mm_setr_pi32(gh, gh);
	__m64 _m_gl = _mm_setr_pi32(gl, gl);

	__m64 _m_16x4, mask_gt_th, mask_ls_tl;

	width <<= 1;
	w3 = width - 3;

	for (i = 0; i < height; i++) {

		SET_Pointers_M64(dataPtr, sourcePtr);

		GROSS_LOOPS_S16;

		for (; j < width; j += 2) {
			pdst_row[j + 0] = THRESH0(psrc_row[j]);
			pdst_row[j + 1] = THRESH1(psrc_row[j + 1]);
		}

		psrc_row += (src_stride >> 1);
		pdst_row += (dst_stride >> 1);
	}
	_m_empty();
}

/* *********************************************************** */

void
mlib_m_ImageThresh4_S163(
    const mlib_s16 *psrc,
    mlib_s32 src_stride,
    mlib_s16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *ghigh,
    const mlib_s32 *glow)
{
	mlib_s32 i, j, k;
	mlib_s32 thigh0, thigh1, thigh2;
	mlib_s32 tlow0, tlow1, tlow2;
	mlib_s32 glow0, glow1, glow2;
	mlib_s32 ghigh0, ghigh1, ghigh2;
	const mlib_s16 *psrc_row = psrc;
	mlib_s16 *pdst_row = pdst;
	__m64 *dataPtr, *sourcePtr;
	__m64 _m_th0, _m_th1, _m_th2, _m_tl0, _m_tl1, _m_tl2;
	__m64 _m_gh0, _m_gh1, _m_gh2, _m_gl0, _m_gl1, _m_gl2;
	__m64 _m_16x4, mask_gt_th, mask_ls_tl;

	{
		mlib_s32 temp10_0 = (thigh[0] & 0xFFFF) | (thigh[1] << 16);
		mlib_s32 temp02_0 = (thigh[2] & 0xFFFF) | (thigh[0] << 16);
		mlib_s32 temp21_0 = (thigh[1] & 0xFFFF) | (thigh[2] << 16);
		mlib_s32 temp10_1 = (tlow[0] & 0xFFFF) | (tlow[1] << 16);
		mlib_s32 temp02_1 = (tlow[2] & 0xFFFF) | (tlow[0] << 16);
		mlib_s32 temp21_1 = (tlow[1] & 0xFFFF) | (tlow[2] << 16);
		mlib_s32 temp10_2 = (ghigh[0] & 0xFFFF) | (ghigh[1] << 16);
		mlib_s32 temp02_2 = (ghigh[2] & 0xFFFF) | (ghigh[0] << 16);
		mlib_s32 temp21_2 = (ghigh[1] & 0xFFFF) | (ghigh[2] << 16);
		mlib_s32 temp10_3 = (glow[0] & 0xFFFF) | (glow[1] << 16);
		mlib_s32 temp02_3 = (glow[2] & 0xFFFF) | (glow[0] << 16);
		mlib_s32 temp21_3 = (glow[1] & 0xFFFF) | (glow[2] << 16);

		_m_th0 =
		    _m_por(_mm_slli_si64(_m_from_int(temp02_0), 32),
		    _m_from_int(temp10_0));
		_m_th1 =
		    _m_por(_mm_slli_si64(_m_from_int(temp10_0), 32),
		    _m_from_int(temp21_0));
		_m_th2 =
		    _m_por(_mm_slli_si64(_m_from_int(temp21_0), 32),
		    _m_from_int(temp02_0));
		_m_tl0 =
		    _m_por(_mm_slli_si64(_m_from_int(temp02_1), 32),
		    _m_from_int(temp10_1));
		_m_tl1 =
		    _m_por(_mm_slli_si64(_m_from_int(temp10_1), 32),
		    _m_from_int(temp21_1));
		_m_tl2 =
		    _m_por(_mm_slli_si64(_m_from_int(temp21_1), 32),
		    _m_from_int(temp02_1));
		_m_gh0 =
		    _m_por(_mm_slli_si64(_m_from_int(temp02_2), 32),
		    _m_from_int(temp10_2));
		_m_gh1 =
		    _m_por(_mm_slli_si64(_m_from_int(temp10_2), 32),
		    _m_from_int(temp21_2));
		_m_gh2 =
		    _m_por(_mm_slli_si64(_m_from_int(temp21_2), 32),
		    _m_from_int(temp02_2));
		_m_gl0 =
		    _m_por(_mm_slli_si64(_m_from_int(temp02_3), 32),
		    _m_from_int(temp10_3));
		_m_gl1 =
		    _m_por(_mm_slli_si64(_m_from_int(temp10_3), 32),
		    _m_from_int(temp21_3));
		_m_gl2 =
		    _m_por(_mm_slli_si64(_m_from_int(temp21_3), 32),
		    _m_from_int(temp02_3));
	}

	INIT_THRESH0(0);
	INIT_THRESH1(1);
	INIT_THRESH2(2);

	width = width * 3;
	src_stride >>= 1;
	dst_stride >>= 1;

	for (i = 0; i < height; i++) {

		SET_Pointers_M64(dataPtr, sourcePtr);

		for (j = 0, k = 0; j < (width - 11); j += 12, k += 3) {
			DO_FAST_THRESH_S16(dataPtr[k + 0], sourcePtr[k + 0],
			    _m_th0, _m_tl0, _m_gh0, _m_gl0);
			DO_FAST_THRESH_S16(dataPtr[k + 1], sourcePtr[k + 1],
			    _m_th1, _m_tl1, _m_gh1, _m_gl1);
			DO_FAST_THRESH_S16(dataPtr[k + 2], sourcePtr[k + 2],
			    _m_th2, _m_tl2, _m_gh2, _m_gl2);
		}

		for (; j < width; j += 3) {
			pdst_row[j] = THRESH0(psrc_row[j]);
			pdst_row[j + 1] = THRESH1(psrc_row[j + 1]);
			pdst_row[j + 2] = THRESH2(psrc_row[j + 2]);
		}
		psrc_row += src_stride;
		pdst_row += dst_stride;
	}
	_m_empty();
}

/* *********************************************************** */

void
mlib_m_ImageThresh4_S164(
    const mlib_s16 *psrc,
    mlib_s32 src_stride,
    mlib_s16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *ghigh,
    const mlib_s32 *glow)
{
	mlib_s32 i, j, k, w3;
	mlib_s32 thigh0 = thigh[0], thigh1 = thigh[1], thigh2 =
	    thigh[2], thigh3 = thigh[3];
	mlib_s32 tlow0 = tlow[0], tlow1 = tlow[1], tlow2 = tlow[2], tlow3 =
	    tlow[3];
	mlib_s32 ghigh0 = ghigh[0], ghigh1 = ghigh[1], ghigh2 =
	    ghigh[2], ghigh3 = ghigh[3];
	mlib_s32 glow0 = glow[0], glow1 = glow[1], glow2 = glow[2], glow3 =
	    glow[3];
	__m64 *dataPtr, *sourcePtr;
	const mlib_s16 *psrc_row = psrc;
	mlib_s16 *pdst_row = pdst;
	__m64 _m_th =
	    _m_por(_mm_slli_si64(_m_from_int((thigh3 << 16) | (thigh2 &
	    0xffff)), 32), _m_from_int((thigh1 << 16) | (thigh0 & 0xffff)));
	__m64 _m_tl =
	    _m_por(_mm_slli_si64(_m_from_int((tlow3 << 16) | (tlow2 & 0xffff)),
	    32), _m_from_int((tlow1 << 16) | (tlow0 & 0xffff)));
	__m64 _m_gh =
	    _m_por(_mm_slli_si64(_m_from_int((ghigh3 << 16) | (ghigh2 &
	    0xffff)), 32), _m_from_int((ghigh1 << 16) | (ghigh0 & 0xffff)));
	__m64 _m_gl =
	    _m_por(_mm_slli_si64(_m_from_int((glow3 << 16) | (glow2 & 0xffff)),
	    32), _m_from_int((glow1 << 16) | (glow0 & 0xffff)));
	__m64 _m_16x4;
	__m64 mask_gt_th;
	__m64 mask_ls_tl;

	width <<= 2;
	w3 = width - 3;

	for (i = 0, j = 0; i < height; i++, j = 0) {

		SET_Pointers_M64(dataPtr, sourcePtr);

		GROSS_LOOPS_S16;

		psrc_row += (src_stride >> 1);
		pdst_row += (dst_stride >> 1);
	}
	_m_empty();
}

/* *********************************************************** */

void
mlib_c_ImageThresh4_U161(
    const mlib_u16 *psrc,
    mlib_s32 src_stride,
    mlib_u16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *ghigh,
    const mlib_s32 *glow)
{
	int i, j;
	mlib_s32 thigh0, tlow0;
	mlib_s32 ghigh0, glow0;

	INIT_THRESH0(0);
	src_stride >>= 1;
	dst_stride >>= 1;

	for (i = 0; i < height; i++) {
		const mlib_u16 *psrc_row = psrc + i * src_stride;
		mlib_u16 *pdst_row = pdst + i * dst_stride;

		for (j = 0; j <= (width - 8); j += 8) {
			pdst_row[j] = THRESH0(psrc_row[j]);
			pdst_row[j + 1] = THRESH0(psrc_row[j + 1]);
			pdst_row[j + 2] = THRESH0(psrc_row[j + 2]);
			pdst_row[j + 3] = THRESH0(psrc_row[j + 3]);
			pdst_row[j + 4] = THRESH0(psrc_row[j + 4]);
			pdst_row[j + 5] = THRESH0(psrc_row[j + 5]);
			pdst_row[j + 6] = THRESH0(psrc_row[j + 6]);
			pdst_row[j + 7] = THRESH0(psrc_row[j + 7]);
		}
		for (; j < width; j++) {
			pdst_row[j] = THRESH0(psrc_row[j]);
		}
	}
}

/* *********************************************************** */

void
mlib_c_ImageThresh4_U162(
    const mlib_u16 *psrc,
    mlib_s32 src_stride,
    mlib_u16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *ghigh,
    const mlib_s32 *glow)
{
	int i, j;
	mlib_s32 thigh0, thigh1;
	mlib_s32 tlow0, tlow1;
	mlib_s32 glow0, glow1;
	mlib_s32 ghigh0, ghigh1;

	INIT_THRESH0(0);
	INIT_THRESH1(1);
	src_stride >>= 1;
	dst_stride >>= 1;
	width <<= 1;

	for (i = 0; i < height; i++) {
		const mlib_u16 *psrc_row = psrc + i * src_stride;
		mlib_u16 *pdst_row = pdst + i * dst_stride;

		for (j = 0; j <= (width - 8); j += 8) {
			pdst_row[j] = THRESH0(psrc_row[j]);
			pdst_row[j + 1] = THRESH1(psrc_row[j + 1]);
			pdst_row[j + 2] = THRESH0(psrc_row[j + 2]);
			pdst_row[j + 3] = THRESH1(psrc_row[j + 3]);
			pdst_row[j + 4] = THRESH0(psrc_row[j + 4]);
			pdst_row[j + 5] = THRESH1(psrc_row[j + 5]);
			pdst_row[j + 6] = THRESH0(psrc_row[j + 6]);
			pdst_row[j + 7] = THRESH1(psrc_row[j + 7]);
		}
		for (; j < width; j += 2) {
			pdst_row[j] = THRESH0(psrc_row[j]);
			pdst_row[j + 1] = THRESH1(psrc_row[j + 1]);
		}
	}
}

/* *********************************************************** */

void
mlib_c_ImageThresh4_U163(
    const mlib_u16 *psrc,
    mlib_s32 src_stride,
    mlib_u16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *ghigh,
    const mlib_s32 *glow)
{
	int i, j;
	mlib_s32 thigh0, thigh1, thigh2;
	mlib_s32 tlow0, tlow1, tlow2;
	mlib_s32 glow0, glow1, glow2;
	mlib_s32 ghigh0, ghigh1, ghigh2;

	INIT_THRESH0(0);
	INIT_THRESH1(1);
	INIT_THRESH2(2);
	width = width * 3;
	src_stride >>= 1;
	dst_stride >>= 1;
	for (i = 0; i < height; i++) {
		const mlib_u16 *psrc_row = psrc + i * src_stride;
		mlib_u16 *pdst_row = pdst + i * dst_stride;

		for (j = 0; j <= (width - 12); j += 12) {
			pdst_row[j] = THRESH0(psrc_row[j]);
			pdst_row[j + 1] = THRESH1(psrc_row[j + 1]);
			pdst_row[j + 2] = THRESH2(psrc_row[j + 2]);
			pdst_row[j + 3] = THRESH0(psrc_row[j + 3]);
			pdst_row[j + 4] = THRESH1(psrc_row[j + 4]);
			pdst_row[j + 5] = THRESH2(psrc_row[j + 5]);
			pdst_row[j + 6] = THRESH0(psrc_row[j + 6]);
			pdst_row[j + 7] = THRESH1(psrc_row[j + 7]);
			pdst_row[j + 8] = THRESH2(psrc_row[j + 8]);
			pdst_row[j + 9] = THRESH0(psrc_row[j + 9]);
			pdst_row[j + 10] = THRESH1(psrc_row[j + 10]);
			pdst_row[j + 11] = THRESH2(psrc_row[j + 11]);
		}
		for (; j < width; j += 3) {
			pdst_row[j] = THRESH0(psrc_row[j]);
			pdst_row[j + 1] = THRESH1(psrc_row[j + 1]);
			pdst_row[j + 2] = THRESH2(psrc_row[j + 2]);
		}
	}
}

/* *********************************************************** */

void
mlib_c_ImageThresh4_U164(
    const mlib_u16 *psrc,
    mlib_s32 src_stride,
    mlib_u16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *ghigh,
    const mlib_s32 *glow)
{
	int i, j;
	mlib_s32 thigh0, thigh1, thigh2, thigh3;
	mlib_s32 tlow0, tlow1, tlow2, tlow3;
	mlib_s32 glow0, glow1, glow2, glow3;
	mlib_s32 ghigh0, ghigh1, ghigh2, ghigh3;

	INIT_THRESH0(0);
	INIT_THRESH1(1);
	INIT_THRESH2(2);
	INIT_THRESH3(3);
	width <<= 2;
	src_stride >>= 1;
	dst_stride >>= 1;
	for (i = 0; i < height; i++) {
		const mlib_u16 *psrc_row = psrc + i * src_stride;
		mlib_u16 *pdst_row = pdst + i * dst_stride;

		for (j = 0; j <= (width - 8); j += 8) {
			pdst_row[j] = THRESH0(psrc_row[j]);
			pdst_row[j + 1] = THRESH1(psrc_row[j + 1]);
			pdst_row[j + 2] = THRESH2(psrc_row[j + 2]);
			pdst_row[j + 3] = THRESH3(psrc_row[j + 3]);
			pdst_row[j + 4] = THRESH0(psrc_row[j + 4]);
			pdst_row[j + 5] = THRESH1(psrc_row[j + 5]);
			pdst_row[j + 6] = THRESH2(psrc_row[j + 6]);
			pdst_row[j + 7] = THRESH3(psrc_row[j + 7]);
		}
		if (j < width) {
			pdst_row[j] = THRESH0(psrc_row[j]);
			pdst_row[j + 1] = THRESH1(psrc_row[j + 1]);
			pdst_row[j + 2] = THRESH2(psrc_row[j + 2]);
			pdst_row[j + 3] = THRESH3(psrc_row[j + 3]);
		}
	}
}

/* *********************************************************** */

#define	DO_THRESH_S32(s0, th, tl, gh, gl)                         \
	(((s0) < (tl)) ? (gl) : (((s0) <= (th)) ? (s0) : (gh)))

#define	THRESH0_S32(s0)	DO_THRESH_S32(s0, thigh0, tlow0, ghigh0, glow0)
#define	THRESH1_S32(s0)	DO_THRESH_S32(s0, thigh1, tlow1, ghigh1, glow1)
#define	THRESH2_S32(s0)	DO_THRESH_S32(s0, thigh2, tlow2, ghigh2, glow2)
#define	THRESH3_S32(s0)	DO_THRESH_S32(s0, thigh3, tlow3, ghigh3, glow3)

/* *********************************************************** */

void
mlib_m_ImageThresh4_S321(
    const mlib_s32 *psrc,
    mlib_s32 src_stride,
    mlib_s32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *ghigh,
    const mlib_s32 *glow)
{
	mlib_s32 i, j, k;
	const mlib_s32 *psrc_row = psrc;
	mlib_s32 *pdst_row = pdst;
	__m64 *dataPtr, *sourcePtr;
	__m64 _m_th = _mm_set1_pi32(thigh[0]);
	__m64 _m_tl = _mm_set1_pi32(tlow[0]);
	__m64 _m_gh = _mm_set1_pi32(ghigh[0]);
	__m64 _m_gl = _mm_set1_pi32(glow[0]);

	src_stride >>= 2;
	dst_stride >>= 2;

	for (i = 0, j = 0; i < height; i++, j = 0) {

		SET_Pointers_M64(dataPtr, sourcePtr);

		for (k = 0; j < (width - 1); j += 2, k++) {

			__m64 save = sourcePtr[k];
			__m64 mask_gt_th = _mm_cmpgt_pi32(save, _m_th);
			__m64 mask_ls_tl = _mm_cmpgt_pi32(_m_tl, save);

			dataPtr[k] =
			    _m_por(_m_pandn(_m_por(mask_gt_th, mask_ls_tl),
			    save), _m_por(_m_pand(_m_gl, mask_ls_tl),
			    _m_pand(_m_gh, mask_gt_th)));
		}

		if (j < width) {
			mlib_s32 source = psrc_row[j];

			pdst_row[j] =
			    (((source) < (tlow[0])) ? (glow[0]) : (((source) <=
			    (thigh[0])) ? (source) : (ghigh[0])));
		}

		psrc_row += src_stride;
		pdst_row += dst_stride;
	}
	_m_empty();
}

/* *********************************************************** */

void
mlib_m_ImageThresh4_S322(
    const mlib_s32 *psrc,
    mlib_s32 src_stride,
    mlib_s32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *ghigh,
    const mlib_s32 *glow)
{
	mlib_s32 i, j, k, w1;
	const mlib_s32 *psrc_row = psrc;
	mlib_s32 *pdst_row = pdst;
	__m64 *dataPtr, *sourcePtr;

	__m64 _m_th = _mm_setr_pi32(thigh[0], thigh[1]);
	__m64 _m_tl = _mm_setr_pi32(tlow[0], tlow[1]);
	__m64 _m_gh = _mm_setr_pi32(ghigh[0], ghigh[1]);
	__m64 _m_gl = _mm_setr_pi32(glow[0], glow[1]);

	src_stride >>= 2;
	dst_stride >>= 2;
	width <<= 1;
	w1 = width - 1;

	for (i = 0, j = 0; i < height; i++, j = 0) {

		SET_Pointers_M64(dataPtr, sourcePtr);

		for (k = 0; j < w1; j += 2, k++) {

			__m64 save = sourcePtr[k];
			__m64 mask_gt_th = _mm_cmpgt_pi32(save, _m_th);
			__m64 mask_ls_tl = _mm_cmpgt_pi32(_m_tl, save);

			dataPtr[k] =
			    _m_por(_m_pandn(_m_por(mask_gt_th, mask_ls_tl),
			    save), _m_por(_m_pand(_m_gl, mask_ls_tl),
			    _m_pand(_m_gh, mask_gt_th)));
		}

		if (j <= w1) {
			mlib_s32 source = psrc_row[j];

			pdst_row[j] =
			    (((source) < (tlow[1])) ? (glow[1]) : (((source) <=
			    (thigh[1])) ? (source) : (ghigh[1])));
		}

		psrc_row += src_stride;
		pdst_row += dst_stride;
	}
	_m_empty();
}

/* *********************************************************** */

void
mlib_m_ImageThresh4_S323(
    const mlib_s32 *psrc,
    mlib_s32 src_stride,
    mlib_s32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *ghigh,
    const mlib_s32 *glow)
{
	mlib_s32 i, j, k, w5;
	mlib_s32 thigh0, thigh1, thigh2;
	mlib_s32 tlow0, tlow1, tlow2;
	mlib_s32 ghigh0, ghigh1, ghigh2;
	mlib_s32 glow0, glow1, glow2;
	const mlib_s32 *psrc_row = psrc;
	mlib_s32 *pdst_row = pdst;
	__m64 *dataPtr, *sourcePtr;
	__m64 _m_th0 =
	    _m_por(_m_from_int(thigh[0]), _mm_slli_si64(_m_from_int(thigh[1]),
	    32));
	__m64 _m_th1 =
	    _m_por(_m_from_int(thigh[2]), _mm_slli_si64(_m_from_int(thigh[0]),
	    32));
	__m64 _m_th2 =
	    _m_por(_m_from_int(thigh[1]), _mm_slli_si64(_m_from_int(thigh[2]),
	    32));
	__m64 _m_tl0 =
	    _m_por(_m_from_int(tlow[0]), _mm_slli_si64(_m_from_int(tlow[1]),
	    32));
	__m64 _m_tl1 =
	    _m_por(_m_from_int(tlow[2]), _mm_slli_si64(_m_from_int(tlow[0]),
	    32));
	__m64 _m_tl2 =
	    _m_por(_m_from_int(tlow[1]), _mm_slli_si64(_m_from_int(tlow[2]),
	    32));
	__m64 _m_gh0 =
	    _m_por(_m_from_int(ghigh[0]), _mm_slli_si64(_m_from_int(ghigh[1]),
	    32));
	__m64 _m_gh1 =
	    _m_por(_m_from_int(ghigh[2]), _mm_slli_si64(_m_from_int(ghigh[0]),
	    32));
	__m64 _m_gh2 =
	    _m_por(_m_from_int(ghigh[1]), _mm_slli_si64(_m_from_int(ghigh[2]),
	    32));
	__m64 _m_gl0 =
	    _m_por(_m_from_int(glow[0]), _mm_slli_si64(_m_from_int(glow[1]),
	    32));
	__m64 _m_gl1 =
	    _m_por(_m_from_int(glow[2]), _mm_slli_si64(_m_from_int(glow[0]),
	    32));
	__m64 _m_gl2 =
	    _m_por(_m_from_int(glow[1]), _mm_slli_si64(_m_from_int(glow[2]),
	    32));

	src_stride >>= 2;
	dst_stride >>= 2;

	INIT_THRESH0(0);
	INIT_THRESH1(1);
	INIT_THRESH2(2);
	width = width * 3;
	w5 = width - 5;

	for (i = 0; i < height; i++) {

		SET_Pointers_M64(dataPtr, sourcePtr);

		for (j = 0, k = 0; j < w5; j += 6, k += 3) {
			__m64 save0 = sourcePtr[k + 0];
			__m64 save1 = sourcePtr[k + 1];
			__m64 save2 = sourcePtr[k + 2];
			__m64 mask_gt_th = _mm_cmpgt_pi32(save0, _m_th0);
			__m64 mask_ls_tl = _mm_cmpgt_pi32(_m_tl0, save0);

			dataPtr[k + 0] =
			    _m_por(_m_pandn(_m_por(mask_gt_th, mask_ls_tl),
			    save0), _m_por(_m_pand(_m_gl0, mask_ls_tl),
			    _m_pand(_m_gh0, mask_gt_th)));
			mask_gt_th = _mm_cmpgt_pi32(save1, _m_th1);
			mask_ls_tl = _mm_cmpgt_pi32(_m_tl1, save1);
			dataPtr[k + 1] =
			    _m_por(_m_pandn(_m_por(mask_gt_th, mask_ls_tl),
			    save1), _m_por(_m_pand(_m_gl1, mask_ls_tl),
			    _m_pand(_m_gh1, mask_gt_th)));
			mask_gt_th = _mm_cmpgt_pi32(save2, _m_th2);
			mask_ls_tl = _mm_cmpgt_pi32(_m_tl2, save2);
			dataPtr[k + 2] =
			    _m_por(_m_pandn(_m_por(mask_gt_th, mask_ls_tl),
			    save2), _m_por(_m_pand(_m_gl2, mask_ls_tl),
			    _m_pand(_m_gh2, mask_gt_th)));
		}
		if (j < width) {
			pdst_row[j] = THRESH0_S32(psrc_row[j]);
			pdst_row[j + 1] = THRESH1_S32(psrc_row[j + 1]);
			pdst_row[j + 2] = THRESH2_S32(psrc_row[j + 2]);
		}

		psrc_row += src_stride;
		pdst_row += dst_stride;
	}
	_m_empty();
}

/* *********************************************************** */

void
mlib_m_ImageThresh4_S324(
    const mlib_s32 *psrc,
    mlib_s32 src_stride,
    mlib_s32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *ghigh,
    const mlib_s32 *glow)
{
	mlib_s32 i, j, k;
	const mlib_s32 *psrc_row = psrc;
	mlib_s32 *pdst_row = pdst;
	__m64 *dataPtr, *sourcePtr;
	__m64 _m_th0 = _mm_setr_pi32(thigh[0], thigh[1]);
	__m64 _m_th1 = _mm_setr_pi32(thigh[2], thigh[3]);
	__m64 _m_tl0 = _mm_setr_pi32(tlow[0], tlow[1]);
	__m64 _m_tl1 = _mm_setr_pi32(tlow[2], tlow[3]);
	__m64 _m_gh0 = _mm_setr_pi32(ghigh[0], ghigh[1]);
	__m64 _m_gh1 = _mm_setr_pi32(ghigh[2], ghigh[3]);
	__m64 _m_gl0 = _mm_setr_pi32(glow[0], glow[1]);
	__m64 _m_gl1 = _mm_setr_pi32(glow[2], glow[3]);

	src_stride >>= 2;
	dst_stride >>= 2;
	width <<= 2;
	for (i = 0; i < height; i++) {

		SET_Pointers_M64(dataPtr, sourcePtr);

		for (j = 0, k = 0; j < width; j += 4, k += 2) {

			__m64 save0 = sourcePtr[k + 0];
			__m64 save1 = sourcePtr[k + 1];
			__m64 mask_gt_th0 = _mm_cmpgt_pi32(save0, _m_th0);
			__m64 mask_ls_tl0 = _mm_cmpgt_pi32(_m_tl0, save0);
			__m64 mask_gt_th1 = _mm_cmpgt_pi32(save1, _m_th1);
			__m64 mask_ls_tl1 = _mm_cmpgt_pi32(_m_tl1, save1);

			dataPtr[k + 0] =
			    _m_por(_m_pandn(_m_por(mask_gt_th0, mask_ls_tl0),
			    save0), _m_por(_m_pand(_m_gl0, mask_ls_tl0),
			    _m_pand(_m_gh0, mask_gt_th0)));
			dataPtr[k + 1] =
			    _m_por(_m_pandn(_m_por(mask_gt_th1, mask_ls_tl1),
			    save1), _m_por(_m_pand(_m_gl1, mask_ls_tl1),
			    _m_pand(_m_gh1, mask_gt_th1)));
		}

		psrc_row += src_stride;
		pdst_row += dst_stride;
	}
	_m_empty();
}

/* *********************************************************** */

mlib_status
__mlib_ImageThresh4(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *ghigh,
    const mlib_s32 *glow)
{
	mlib_s32 sstride, dstride, width, height, nchan;
	mlib_type dtype;
	void *psrc, *pdst;
	mlib_s32 th[4], tl[4], gl[4], gh[4];
	mlib_s32 i, j;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_FULL_EQUAL(src, dst);

	dtype = mlib_ImageGetType(src);
	nchan = mlib_ImageGetChannels(src);
	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	sstride = mlib_ImageGetStride(src);
	dstride = mlib_ImageGetStride(dst);
	psrc = mlib_ImageGetData(src);
	pdst = mlib_ImageGetData(dst);

	for (i = 0; i < nchan; i++) {
		if (thigh[i] < tlow[i])
			return (MLIB_FAILURE);
	}

	if (dtype == MLIB_BYTE) {

		mlib_u8 th_u8[24], tl_u8[24], gl_u8[24], gh_u8[24];

		for (i = 0; i < nchan; i++) {
			gl[i] = glow[i] & 255;
			gh[i] = ghigh[i] & 255;
			tl[i] = tlow[i];
			th[i] = thigh[i];
			if (th[i] < 0)
				gl[i] = th[i] = tl[i] = gh[i];
			else if (tl[i] > MLIB_U8_MAX)
				gh[i] = th[i] = tl[i] = gl[i];
			else {
				if (th[i] > MLIB_U8_MAX)
					th[i] = MLIB_U8_MAX;
				if (tl[i] < 0)
					tl[i] = 0;
			}
		}

		for (j = 0; j < 24; j += i) {
			for (i = 0; i < nchan; i++) {
				gl_u8[j + i] = (mlib_u8)gl[i];
				gh_u8[j + i] = (mlib_u8)gh[i];
				tl_u8[j + i] = (mlib_u8)tl[i];
				th_u8[j + i] = (mlib_u8)th[i];
			}
		}

		switch (nchan) {

		case 1:
			mlib_m_ImageThresh4_U81((mlib_u8 *)psrc, sstride,
			    (mlib_u8 *)pdst, dstride, width, height, th_u8,
			    tl_u8, gh_u8, gl_u8);
			break;
		case 2:
			mlib_m_ImageThresh4_U82((mlib_u8 *)psrc, sstride,
			    (mlib_u8 *)pdst, dstride, width, height, th_u8,
			    tl_u8, gh_u8, gl_u8);
			break;
		case 3:
			mlib_m_ImageThresh4_U83((mlib_u8 *)psrc, sstride,
			    (mlib_u8 *)pdst, dstride, width, height, th_u8,
			    tl_u8, gh_u8, gl_u8);
			break;
		case 4:
			mlib_m_ImageThresh4_U84((mlib_u8 *)psrc, sstride,
			    (mlib_u8 *)pdst, dstride, width, height, th_u8,
			    tl_u8, gh_u8, gl_u8);
			break;
		}
		return (MLIB_SUCCESS);

	} else if (dtype == MLIB_SHORT) {

		for (i = 0; i < nchan; i++) {
			gl[i] = (glow[i] << 16) >> 16;
			gh[i] = (ghigh[i] << 16) >> 16;
			tl[i] = tlow[i];
			th[i] = thigh[i];
			if (th[i] < MLIB_S16_MIN)
				gl[i] = th[i] = tl[i] = gh[i];
			else if (tl[i] > MLIB_S16_MAX)
				gh[i] = th[i] = tl[i] = gl[i];
			else {
				if (th[i] > MLIB_S16_MAX)
					th[i] = MLIB_S16_MAX;
				if (tl[i] < MLIB_S16_MIN)
					tl[i] = MLIB_S16_MIN;
			}
		}

		switch (nchan) {
		case 1:
			mlib_m_ImageThresh4_S161((mlib_s16 *)psrc, sstride,
			    (mlib_s16 *)pdst, dstride, width, height, th, tl,
			    gh, gl);
			break;
		case 2:
			mlib_m_ImageThresh4_S162((mlib_s16 *)psrc, sstride,
			    (mlib_s16 *)pdst, dstride, width, height, th, tl,
			    gh, gl);
			break;
		case 3:
			mlib_m_ImageThresh4_S163((mlib_s16 *)psrc, sstride,
			    (mlib_s16 *)pdst, dstride, width, height, th, tl,
			    gh, gl);
			break;
		case 4:
			mlib_m_ImageThresh4_S164((mlib_s16 *)psrc, sstride,
			    (mlib_s16 *)pdst, dstride, width, height, th, tl,
			    gh, gl);
			break;
		}
		return (MLIB_SUCCESS);

	} else if (dtype == MLIB_USHORT) {

		for (i = 0; i < nchan; i++) {
			gl[i] = (glow[i] << 16) >> 16;
			gh[i] = (ghigh[i] << 16) >> 16;
			tl[i] = tlow[i];
			th[i] = thigh[i];
			if (th[i] < MLIB_U16_MIN)
				gl[i] = th[i] = tl[i] = gh[i];
			else if (tl[i] > MLIB_U16_MAX)
				gh[i] = th[i] = tl[i] = gl[i];
			else {
				if (th[i] > MLIB_U16_MAX)
					th[i] = MLIB_U16_MAX;
				if (tl[i] < MLIB_U16_MIN)
					tl[i] = MLIB_U16_MIN;
			}
		}

		switch (nchan) {
		case 1:
			mlib_c_ImageThresh4_U161(psrc, sstride,
			    pdst, dstride, width, height, th, tl, gh, gl);
			break;
		case 2:
			mlib_c_ImageThresh4_U162(psrc, sstride,
			    pdst, dstride, width, height, th, tl, gh, gl);
			break;
		case 3:
			mlib_c_ImageThresh4_U163(psrc, sstride,
			    pdst, dstride, width, height, th, tl, gh, gl);
			break;
		case 4:
			mlib_c_ImageThresh4_U164(psrc, sstride,
			    pdst, dstride, width, height, th, tl, gh, gl);
			break;
		}
		return (MLIB_SUCCESS);

	} else if (dtype == MLIB_INT) {

		switch (nchan) {
		case 1:
			mlib_m_ImageThresh4_S321((mlib_s32 *)psrc, sstride,
			    (mlib_s32 *)pdst, dstride, width, height, thigh,
			    tlow, ghigh, glow);
			break;
		case 2:
			mlib_m_ImageThresh4_S322((mlib_s32 *)psrc, sstride,
			    (mlib_s32 *)pdst, dstride, width, height, thigh,
			    tlow, ghigh, glow);
			break;
		case 3:
			mlib_m_ImageThresh4_S323((mlib_s32 *)psrc, sstride,
			    (mlib_s32 *)pdst, dstride, width, height, thigh,
			    tlow, ghigh, glow);
			break;
		case 4:
			mlib_m_ImageThresh4_S324((mlib_s32 *)psrc, sstride,
			    (mlib_s32 *)pdst, dstride, width, height, thigh,
			    tlow, ghigh, glow);
			break;
		}
		return (MLIB_SUCCESS);
	}

	return (MLIB_FAILURE);
}

/* *********************************************************** */
