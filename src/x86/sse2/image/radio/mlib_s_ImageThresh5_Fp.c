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

#pragma ident   "@(#)mlib_s_ImageThresh5_Fp.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageThresh5_Fp - thresholding
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageThresh5_Fp(mlib_image       *dst,
 *                                       const mlib_image *src,
 *                                       const mlib_d64   *thigh,
 *                                       const mlib_d64   *tlow,
 *                                       const mlib_d64   *gmid);
 *
 * ARGUMENT
 *      dst     pointer to output image
 *      src     pointer to input image
 *      thigh   array of high thresholds
 *      tlow    array of low thresholds
 *      gmid    array of values between thresholds
 *
 * RESTRICTION
 *      The images must have the same type, the same size, and the same number
 *      of channels.
 *      The images can have 1, 2, 3, or 4 channels.
 *      The images can be in MLIB_FLOAT or MLIB_DOUBLE data type.
 *
 * DESCRIPTION
 *      If the pixel channel value is below the thigh for that channel
 *      and the pixel channel value is above the tlow for that channel,
 *      set the destination to the gmid value for that channel.
 *
 *                      +- src[x][y][c] src[x][y][c] < tlow[c]
 *                      |
 *      dst[x][y][c]  = +- gmid[c]      tlow[c] <= src[x][y][c] <= thigh[c]
 *                      |
 *                      +- src[x][y][c] src[x][y][c] > thigh[c]
 */

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

#pragma weak mlib_ImageThresh5_Fp = __mlib_ImageThresh5_Fp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageThresh5_Fp) mlib_ImageThresh5_Fp
    __attribute__((weak, alias("__mlib_ImageThresh5_Fp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static void mlib_s_ImageThresh5_F321(
    const mlib_f32 *psrc,
    mlib_s32 src_stride,
    mlib_f32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thigh,
    const mlib_d64 *tlow,
    const mlib_d64 *gmid);

static void mlib_s_ImageThresh5_F322(
    const mlib_f32 *psrc,
    mlib_s32 src_stride,
    mlib_f32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thigh,
    const mlib_d64 *tlow,
    const mlib_d64 *gmid);

static void mlib_s_ImageThresh5_F323(
    const mlib_f32 *psrc,
    mlib_s32 src_stride,
    mlib_f32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thigh,
    const mlib_d64 *tlow,
    const mlib_d64 *gmid);

static void mlib_s_ImageThresh5_F324(
    const mlib_f32 *psrc,
    mlib_s32 src_stride,
    mlib_f32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thigh,
    const mlib_d64 *tlow,
    const mlib_d64 *gmid);

static void mlib_s_ImageThresh5_D641(
    const mlib_d64 *psrc,
    mlib_s32 src_stride,
    mlib_d64 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thigh,
    const mlib_d64 *tlow,
    const mlib_d64 *gmid);

static void mlib_s_ImageThresh5_D642(
    const mlib_d64 *psrc,
    mlib_s32 src_stride,
    mlib_d64 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thigh,
    const mlib_d64 *tlow,
    const mlib_d64 *gmid);

static void mlib_s_ImageThresh5_D643(
    const mlib_d64 *psrc,
    mlib_s32 src_stride,
    mlib_d64 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thigh,
    const mlib_d64 *tlow,
    const mlib_d64 *gmid);

static void mlib_s_ImageThresh5_D644(
    const mlib_d64 *psrc,
    mlib_s32 src_stride,
    mlib_d64 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thigh,
    const mlib_d64 *tlow,
    const mlib_d64 *gmid);

/* *********************************************************** */
#ifdef DTYPE
#undef DTYPE
#endif
#ifdef STYPE
#undef STYPE
#endif

#define	DTYPE  mlib_f32
#define	STYPE  __m128
/* *********************************************************** */
#define	INIT_THRESH0(n)                                       \
	thigh0 = (DTYPE)thigh[(n)];                             \
	tlow0 = (DTYPE)tlow[(n)];                               \
	gmid0 = gmid[(n)];

#define	INIT_THRESH1(n)                                       \
	thigh1 = (DTYPE)thigh[(n)];                             \
	tlow1 = (DTYPE)tlow[(n)];                               \
	gmid1 = gmid[(n)];

#define	INIT_THRESH2(n)                                       \
	thigh2 = (DTYPE)thigh[(n)];                             \
	tlow2 = (DTYPE)tlow[(n)];                               \
	gmid2 = gmid[(n)];

#define	INIT_THRESH3(n)                                       \
	thigh3 = (DTYPE)thigh[(n)];                             \
	tlow3 = (DTYPE)tlow[(n)];                               \
	gmid3 = gmid[(n)];

/* *********************************************************** */

#define	SET_Pointers_PS(dst, src)                           \
	dst = (DTYPE *) ((mlib_addr)pdst_row);                  \
	src = (DTYPE *) ((mlib_addr)psrc_row);

/* *********************************************************** */

#define	DO_THRESH_F32(s0, th, tl, gm)                         \
	(((s0) < (tl)) ? (s0) : (((s0) <= (th)) ? (gm) : (s0)))

#define	THRESH0_F32(s0)	DO_THRESH_F32(s0, thigh0, tlow0, gmid0)
#define	THRESH1_F32(s0)	DO_THRESH_F32(s0, thigh1, tlow1, gmid1)
#define	THRESH2_F32(s0)	DO_THRESH_F32(s0, thigh2, tlow2, gmid2)
#define	THRESH3_F32(s0)	DO_THRESH_F32(s0, thigh3, tlow3, gmid3)

/* *********************************************************** */
#define	DO_FAST_THRESH_F32(dst, src, th_SSE2, tl_SSE2, gm_SSE2) \
	_s_32x4 = _mm_loadu_ps(src);                                \
	mask_gt_th = _mm_cmpgt_ps(_s_32x4, th_SSE2);                \
	mask_ls_tl = _mm_cmpgt_ps(tl_SSE2, _s_32x4);                \
	mask = _mm_or_ps(mask_gt_th, mask_ls_tl);                   \
	dd =                                                        \
		_mm_or_ps(_mm_andnot_ps(mask, gm_SSE2),                 \
		_mm_and_ps(mask, _s_32x4));                             \
	_mm_storeu_ps(dst, dd);                                     \
	dst += 4;                                                   \
	src += 4
/* *********************************************************** */

#define	GROSS_LOOPS_F32                                         \
	for (j = 0; j < w3; j += 4) {                               \
	    DO_FAST_THRESH_F32(dataPtr, sourcePtr,                  \
			_s_th, _s_tl, _s_gm);                               \
	}
/* *********************************************************** */


void
mlib_s_ImageThresh5_F321(
    const DTYPE *psrc,
    mlib_s32 src_stride,
    DTYPE *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_d64 *thigh,
	const mlib_d64 *tlow,
	const mlib_d64 *gmid)
{
	mlib_s32 i, j, k, w3 = width - 3;
	const DTYPE *psrc_row = psrc;
	DTYPE thigh0, tlow0, gmid0;
	DTYPE *pdst_row = pdst;
	DTYPE *dataPtr, *sourcePtr;
	STYPE _s_th = _mm_set1_ps((DTYPE)thigh[0]);
	STYPE _s_tl = _mm_set1_ps((DTYPE)tlow[0]);
	STYPE _s_gm = _mm_set1_ps((DTYPE)gmid[0]);
	STYPE _s_32x4, mask_gt_th, mask_ls_tl, mask, dd;

	INIT_THRESH0(0);

	for (i = 0, j = 0; i < height; i++, j = 0) {

		SET_Pointers_PS(dataPtr, sourcePtr);
		GROSS_LOOPS_F32;
		for (; j < width; j++)
			pdst_row[j] = THRESH0_F32(psrc_row[j]);

		psrc_row += src_stride;
		pdst_row += dst_stride;
	}
}

/* *********************************************************** */

void
mlib_s_ImageThresh5_F322(
    const DTYPE *psrc,
    mlib_s32 src_stride,
    DTYPE *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_d64 *thigh,
	const mlib_d64 *tlow,
	const mlib_d64 *gmid)
{
	mlib_s32 i, j, k, w3 = width;
	const DTYPE *psrc_row = psrc;
	DTYPE *pdst_row = pdst;
	DTYPE thigh0, thigh1;
	DTYPE tlow0, tlow1;
	DTYPE gmid0, gmid1;

	DTYPE *dataPtr, *sourcePtr;
	STYPE _s_th = _mm_setr_ps(
			(DTYPE)thigh[0], (DTYPE)thigh[1],
			(DTYPE)thigh[0], (DTYPE)thigh[1]);
	STYPE _s_tl = _mm_setr_ps(
			(DTYPE)tlow[0], (DTYPE)tlow[1],
			(DTYPE)tlow[0], (DTYPE)tlow[1]);
	STYPE _s_gm = _mm_setr_ps(
			(DTYPE)gmid[0], (DTYPE)gmid[1],
			(DTYPE)gmid[0], (DTYPE)gmid[1]);
	STYPE _s_32x4, mask_gt_th, mask_ls_tl, mask, dd;

	width <<= 1;
	w3 = width - 3;

	INIT_THRESH0(0);
	INIT_THRESH1(1);

	for (i = 0, j = 0; i < height; i++, j = 0) {

		SET_Pointers_PS(dataPtr, sourcePtr);
		GROSS_LOOPS_F32;
		for (; j < width; j += 2) {
			pdst_row[j + 0] = THRESH0_F32(psrc_row[j + 0]);
			pdst_row[j + 1] = THRESH1_F32(psrc_row[j + 1]);
		}

		psrc_row += src_stride;
		pdst_row += dst_stride;
	}
}

/* *********************************************************** */

void
mlib_s_ImageThresh5_F323(
    const DTYPE *psrc,
    mlib_s32 src_stride,
    DTYPE *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_d64 *thigh,
	const mlib_d64 *tlow,
	const mlib_d64 *gmid)
{
	mlib_s32 i, j, k;
	DTYPE thigh0, thigh1, thigh2;
	DTYPE tlow0, tlow1, tlow2;
	DTYPE gmid0, gmid1, gmid2;
	const DTYPE *psrc_row = psrc;
	DTYPE *pdst_row = pdst;
	DTYPE *dataPtr, *sourcePtr;
	STYPE _s_th0 = _mm_setr_ps(
		(DTYPE)thigh[0], (DTYPE)thigh[1],
		(DTYPE)thigh[2], (DTYPE)thigh[0]);
	STYPE _s_th1 = _mm_setr_ps(
		(DTYPE)thigh[1], (DTYPE)thigh[2],
		(DTYPE)thigh[0], (DTYPE)thigh[1]);
	STYPE _s_th2 = _mm_setr_ps(
		(DTYPE)thigh[2], (DTYPE)thigh[0],
		(DTYPE)thigh[1], (DTYPE)thigh[2]);
	STYPE _s_tl0 = _mm_setr_ps(
		(DTYPE)tlow[0], (DTYPE)tlow[1],
		(DTYPE)tlow[2], (DTYPE)tlow[0]);
	STYPE _s_tl1 = _mm_setr_ps(
		(DTYPE)tlow[1], (DTYPE)tlow[2],
		(DTYPE)tlow[0], (DTYPE)tlow[1]);
	STYPE _s_tl2 = _mm_setr_ps(
		(DTYPE)tlow[2], (DTYPE)tlow[0],
		(DTYPE)tlow[1], (DTYPE)tlow[2]);
	STYPE _s_gm0 = _mm_setr_ps(
		(DTYPE)gmid[0], (DTYPE)gmid[1],
		(DTYPE)gmid[2], (DTYPE)gmid[0]);
	STYPE _s_gm1 = _mm_setr_ps(
		(DTYPE)gmid[1], (DTYPE)gmid[2],
		(DTYPE)gmid[0], (DTYPE)gmid[1]);
	STYPE _s_gm2 = _mm_setr_ps(
		(DTYPE)gmid[2], (DTYPE)gmid[0],
		(DTYPE)gmid[1], (DTYPE)gmid[2]);
	STYPE _s_32x4, mask_gt_th, mask_ls_tl, mask, dd;

	INIT_THRESH0(0);
	INIT_THRESH1(1);
	INIT_THRESH2(2);
	width = width * 3;

	for (i = 0; i < height; i++) {

		SET_Pointers_PS(dataPtr, sourcePtr);

		for (j = 0; j < (width - 11); j += 12) {
			DO_FAST_THRESH_F32(dataPtr, sourcePtr,
			    _s_th0, _s_tl0, _s_gm0);
			DO_FAST_THRESH_F32(dataPtr, sourcePtr,
			    _s_th1, _s_tl1, _s_gm1);
			DO_FAST_THRESH_F32(dataPtr, sourcePtr,
			    _s_th2, _s_tl2, _s_gm2);
		}

		for (; j < width; j += 3) {
			pdst_row[j] = THRESH0_F32(psrc_row[j]);
			pdst_row[j + 1] = THRESH1_F32(psrc_row[j + 1]);
			pdst_row[j + 2] = THRESH2_F32(psrc_row[j + 2]);
		}

		psrc_row += src_stride;
		pdst_row += dst_stride;
	}
}

/* *********************************************************** */

void
mlib_s_ImageThresh5_F324(
    const DTYPE *psrc,
    mlib_s32 src_stride,
    DTYPE *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_d64 *thigh,
	const mlib_d64 *tlow,
	const mlib_d64 *gmid)
{
	mlib_s32 i, j, k, w3;
	const DTYPE *psrc_row = psrc;
	DTYPE *pdst_row = pdst;

	DTYPE *dataPtr, *sourcePtr;
	STYPE _s_th = _mm_setr_ps(
			(DTYPE)thigh[0], (DTYPE)thigh[1],
			(DTYPE)thigh[2], (DTYPE)thigh[3]);
	STYPE _s_tl = _mm_setr_ps(
			(DTYPE)tlow[0], (DTYPE)tlow[1],
			(DTYPE)tlow[2], (DTYPE)tlow[3]);
	STYPE _s_gm = _mm_setr_ps(
			(DTYPE)gmid[0], (DTYPE)gmid[1],
			(DTYPE)gmid[2], (DTYPE)gmid[3]);
	STYPE _s_32x4, mask_gt_th, mask_ls_tl, mask, dd;

	width <<= 2;
	w3 = width - 3;

	for (i = 0; i < height; i++) {

		SET_Pointers_PS(dataPtr, sourcePtr);
		GROSS_LOOPS_F32;
		psrc_row += src_stride;
		pdst_row += dst_stride;
	}
}

/* *********************************************************** */
#ifdef DTYPE
#undef DTYPE
#endif
#ifdef STYPE
#undef STYPE
#endif

#define	DTYPE  mlib_d64
#define	STYPE  __m128d
/* *********************************************************** */
#define	INIT_THRESH0(n)                                       \
	thigh0 = (DTYPE)thigh[(n)];                             \
	tlow0 = (DTYPE)tlow[(n)];                               \
	gmid0 = gmid[(n)];

#define	INIT_THRESH1(n)                                       \
	thigh1 = (DTYPE)thigh[(n)];                             \
	tlow1 = (DTYPE)tlow[(n)];                               \
	gmid1 = gmid[(n)];

#define	INIT_THRESH2(n)                                       \
	thigh2 = (DTYPE)thigh[(n)];                             \
	tlow2 = (DTYPE)tlow[(n)];                               \
	gmid2 = gmid[(n)];

#define	INIT_THRESH3(n)                                       \
	thigh3 = (DTYPE)thigh[(n)];                             \
	tlow3 = (DTYPE)tlow[(n)];                               \
	gmid3 = gmid[(n)];

/* *********************************************************** */

#define	SET_Pointers_PD(dst, src)                           \
	dst = (DTYPE *) ((mlib_addr)pdst_row);                  \
	src = (DTYPE *) ((mlib_addr)psrc_row);

/* *********************************************************** */

#define	DO_THRESH_D64(s0, th, tl, gm)                       \
	(((s0) < (tl)) ? (s0) : (((s0) <= (th)) ? (gm) : (s0)))

#define	THRESH0_D64(s0)	DO_THRESH_D64(s0, thigh0, tlow0, gmid0)
#define	THRESH1_D64(s0)	DO_THRESH_D64(s0, thigh1, tlow1, gmid1)
#define	THRESH2_D64(s0)	DO_THRESH_D64(s0, thigh2, tlow2, gmid2)
#define	THRESH3_D64(s0)	DO_THRESH_D64(s0, thigh3, tlow3, gmid3)

/* *********************************************************** */
#define	DO_FAST_THRESH_D64(dst, src, th_SSE2, tl_SSE2, gm_SSE2) \
	_s_64x2 = _mm_loadu_pd(src);                                \
	mask_gt_th = _mm_cmpgt_pd(_s_64x2, th_SSE2);                \
	mask_ls_tl = _mm_cmpgt_pd(tl_SSE2, _s_64x2);                \
	mask = _mm_or_pd(mask_gt_th, mask_ls_tl);                   \
	dd =                                                        \
		_mm_or_pd(_mm_andnot_pd(mask, gm_SSE2),                 \
		_mm_and_pd(mask, _s_64x2));                             \
	_mm_storeu_pd(dst, dd);                                     \
	dst += 2;                                                   \
	src += 2
/* *********************************************************** */

#define	GROSS_LOOPS_D64                                         \
	for (j = 0; j < w1; j += 2) {                               \
	    DO_FAST_THRESH_D64(dataPtr, sourcePtr,                  \
			_s_th, _s_tl, _s_gm);                               \
	}
/* *********************************************************** */


void
mlib_s_ImageThresh5_D641(
    const DTYPE *psrc,
    mlib_s32 src_stride,
    DTYPE *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_d64 *thigh,
	const mlib_d64 *tlow,
	const mlib_d64 *gmid)
{
	mlib_s32 i, j, k, w1 = width - 1, tail = (width & 1);
	const DTYPE *psrc_row = psrc;
	DTYPE thigh0, tlow0, gmid0;
	DTYPE *pdst_row = pdst;
	DTYPE *dataPtr, *sourcePtr;
	STYPE _s_th = _mm_set1_pd((DTYPE)thigh[0]);
	STYPE _s_tl = _mm_set1_pd((DTYPE)tlow[0]);
	STYPE _s_gm = _mm_set1_pd((DTYPE)gmid[0]);
	STYPE _s_64x2, mask_gt_th, mask_ls_tl, mask, dd;

	INIT_THRESH0(0);

	for (i = 0, j = 0; i < height; i++, j = 0) {

		SET_Pointers_PD(dataPtr, sourcePtr);
		GROSS_LOOPS_D64;
		if (tail)
			pdst_row[j] = THRESH0_D64(psrc_row[j]);

		psrc_row += src_stride;
		pdst_row += dst_stride;
	}
}

/* *********************************************************** */

void
mlib_s_ImageThresh5_D642(
    const DTYPE *psrc,
    mlib_s32 src_stride,
    DTYPE *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_d64 *thigh,
	const mlib_d64 *tlow,
	const mlib_d64 *gmid)
{
	mlib_s32 i, j, k, w1;
	const DTYPE *psrc_row = psrc;
	DTYPE *pdst_row = pdst;

	DTYPE *dataPtr, *sourcePtr;
	STYPE _s_th = _mm_setr_pd(
			(DTYPE)thigh[0], (DTYPE)thigh[1]);
	STYPE _s_tl = _mm_setr_pd(
			(DTYPE)tlow[0], (DTYPE)tlow[1]);
	STYPE _s_gm = _mm_setr_pd(
			(DTYPE)gmid[0], (DTYPE)gmid[1]);
	STYPE _s_64x2, mask_gt_th, mask_ls_tl, mask, dd;

	width <<= 1;
	w1 = width;

	for (i = 0, j = 0; i < height; i++, j = 0) {

		SET_Pointers_PD(dataPtr, sourcePtr);
		GROSS_LOOPS_D64;

		psrc_row += src_stride;
		pdst_row += dst_stride;
	}
}

/* *********************************************************** */

void
mlib_s_ImageThresh5_D643(
    const DTYPE *psrc,
    mlib_s32 src_stride,
    DTYPE *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_d64 *thigh,
	const mlib_d64 *tlow,
	const mlib_d64 *gmid)
{
	mlib_s32 i, j, k;
	DTYPE thigh0, thigh1, thigh2;
	DTYPE tlow0, tlow1, tlow2;
	DTYPE gmid0, gmid1, gmid2;
	const DTYPE *psrc_row = psrc;
	DTYPE *pdst_row = pdst;
	DTYPE *dataPtr, *sourcePtr;
	STYPE _s_th0 = _mm_setr_pd(
		(DTYPE)thigh[0], (DTYPE)thigh[1]);
	STYPE _s_th1 = _mm_setr_pd(
		(DTYPE)thigh[2], (DTYPE)thigh[0]);
	STYPE _s_th2 = _mm_setr_pd(
		(DTYPE)thigh[1], (DTYPE)thigh[2]);
	STYPE _s_tl0 = _mm_setr_pd(
		(DTYPE)tlow[0], (DTYPE)tlow[1]);
	STYPE _s_tl1 = _mm_setr_pd(
		(DTYPE)tlow[2], (DTYPE)tlow[0]);
	STYPE _s_tl2 = _mm_setr_pd(
		(DTYPE)tlow[1], (DTYPE)tlow[2]);
	STYPE _s_gm0 = _mm_setr_pd(
		(DTYPE)gmid[0], (DTYPE)gmid[1]);
	STYPE _s_gm1 = _mm_setr_pd(
		(DTYPE)gmid[2], (DTYPE)gmid[0]);
	STYPE _s_gm2 = _mm_setr_pd(
		(DTYPE)gmid[1], (DTYPE)gmid[2]);
	STYPE _s_64x2, mask_gt_th, mask_ls_tl, mask, dd;

	INIT_THRESH0(0);
	INIT_THRESH1(1);
	INIT_THRESH2(2);
	width = width * 3;

	for (i = 0; i < height; i++) {

		SET_Pointers_PD(dataPtr, sourcePtr);

		for (j = 0; j < (width - 5); j += 6) {
			DO_FAST_THRESH_D64(dataPtr, sourcePtr,
			    _s_th0, _s_tl0, _s_gm0);
			DO_FAST_THRESH_D64(dataPtr, sourcePtr,
			    _s_th1, _s_tl1, _s_gm1);
			DO_FAST_THRESH_D64(dataPtr, sourcePtr,
			    _s_th2, _s_tl2, _s_gm2);
		}

		for (; j < width; j += 3) {
			pdst_row[j] = THRESH0_D64(psrc_row[j]);
			pdst_row[j + 1] = THRESH1_D64(psrc_row[j + 1]);
			pdst_row[j + 2] = THRESH2_D64(psrc_row[j + 2]);
		}

		psrc_row += src_stride;
		pdst_row += dst_stride;
	}
}

/* *********************************************************** */

void
mlib_s_ImageThresh5_D644(
    const DTYPE *psrc,
    mlib_s32 src_stride,
    DTYPE *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
	const mlib_d64 *thigh,
	const mlib_d64 *tlow,
	const mlib_d64 *gmid)
{
	mlib_s32 i, j, k;
	const DTYPE *psrc_row = psrc;
	DTYPE *pdst_row = pdst;

	DTYPE *dataPtr, *sourcePtr;
	STYPE _s_th0 = _mm_setr_pd(
			(DTYPE)thigh[0], (DTYPE)thigh[1]);
	STYPE _s_th1 = _mm_setr_pd(
			(DTYPE)thigh[2], (DTYPE)thigh[3]);
	STYPE _s_tl0 = _mm_setr_pd(
			(DTYPE)tlow[0], (DTYPE)tlow[1]);
	STYPE _s_tl1 = _mm_setr_pd(
			(DTYPE)tlow[2], (DTYPE)tlow[3]);
	STYPE _s_gm0 = _mm_setr_pd(
			(DTYPE)gmid[0], (DTYPE)gmid[1]);
	STYPE _s_gm1 = _mm_setr_pd(
			(DTYPE)gmid[2], (DTYPE)gmid[3]);
	STYPE _s_64x2, mask_gt_th, mask_ls_tl, mask, dd;

	width <<= 2;

	for (i = 0; i < height; i++) {

		SET_Pointers_PD(dataPtr, sourcePtr);
		for (j = 0; j < (width - 3); j += 4) {
			DO_FAST_THRESH_D64(dataPtr, sourcePtr,
			    _s_th0, _s_tl0, _s_gm0);
			DO_FAST_THRESH_D64(dataPtr, sourcePtr,
			    _s_th1, _s_tl1, _s_gm1);
		}

		psrc_row += src_stride;
		pdst_row += dst_stride;
	}
}


/* *********************************************************** */

mlib_status
__mlib_ImageThresh5_Fp(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *thigh,
    const mlib_d64 *tlow,
    const mlib_d64 *gmid)
{
	mlib_s32 sstride, dstride, width, height, nchan;
	mlib_type dtype;
	void *psrc, *pdst;
	mlib_s32 i;

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

	if (dtype == MLIB_FLOAT)
		sstride >>= 2;
	else if (dtype == MLIB_DOUBLE)
		sstride >>= 3;
	else
		return (MLIB_FAILURE);

	if (dtype == MLIB_FLOAT)
		dstride >>= 2;
	else if (dtype == MLIB_DOUBLE)
		dstride >>= 3;
	else
		return (MLIB_FAILURE);

	if (dtype == MLIB_FLOAT) {

		switch (nchan) {
		case 1:
			mlib_s_ImageThresh5_F321(psrc, sstride,
			    pdst, dstride, width, height, thigh, tlow, gmid);
			break;
		case 2:
			mlib_s_ImageThresh5_F322(psrc, sstride,
			    pdst, dstride, width, height, thigh, tlow, gmid);
			break;
		case 3:
			mlib_s_ImageThresh5_F323(psrc, sstride,
			    pdst, dstride, width, height, thigh, tlow, gmid);
			break;
		case 4:
			mlib_s_ImageThresh5_F324(psrc, sstride,
			    pdst, dstride, width, height, thigh, tlow, gmid);
			break;
		}

		return (MLIB_SUCCESS);
	} else if (dtype == MLIB_DOUBLE) {

		switch (nchan) {
		case 1:
			mlib_s_ImageThresh5_D641(psrc, sstride,
			    pdst, dstride, width, height, thigh, tlow, gmid);
			break;
		case 2:
			mlib_s_ImageThresh5_D642(psrc, sstride,
			    pdst, dstride, width, height, thigh, tlow, gmid);
			break;
		case 3:
			mlib_s_ImageThresh5_D643(psrc, sstride,
			    pdst, dstride, width, height, thigh, tlow, gmid);
			break;
		case 4:
			mlib_s_ImageThresh5_D644(psrc, sstride,
			    pdst, dstride, width, height, thigh, tlow, gmid);
			break;
		}

		return (MLIB_SUCCESS);
	}

	return (MLIB_FAILURE);
}

/* *********************************************************** */
