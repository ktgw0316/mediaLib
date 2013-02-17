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

#pragma ident   "@(#)mlib_s_ImageThresh1_Fp.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageThresh1_Fp - thresholding
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageThresh1_Fp(mlib_image       *dst,
 *                                       const mlib_image *src,
 *                                       const mlib_d64   *thresh,
 *                                       const mlib_d64   *ghigh,
 *                                       const mlib_d64   *glow);
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
 *      The images can be in MLIB_FLOAT or MLIB_DOUBLE data type.
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

#pragma weak mlib_ImageThresh1_Fp = __mlib_ImageThresh1_Fp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageThresh1_Fp) mlib_ImageThresh1_Fp
    __attribute__((weak, alias("__mlib_ImageThresh1_Fp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */
#define	PARAMS_BIT                                            \
	void *psrc, void *pdst, mlib_s32 src_stride,            \
	mlib_s32 dst_stride, mlib_s32 width, mlib_s32 height,   \
	void *__thresh, void *__ghigh, void *__glow,            \
	mlib_s32 dbit_off

#define	PARAMS_SSE2                                           \
	void *psrc, mlib_s32 src_stride,                        \
	void *pdst, mlib_s32 dst_stride, mlib_s32 width,        \
	mlib_s32 height, const mlib_d64 *thresh,                \
	const mlib_d64 *ghigh, const mlib_d64 *glow

#define	CALL_PARAMS_BIT                                     \
	psrc, pdst, sstride, dstride, width,                    \
	height, (void *)thresh, (void *)ghigh,                  \
	(void *)glow, dbit_off

#define	CALL_PARAMS_SSE2                                    \
	psrc, sstride, pdst, dstride, width,                    \
	height, thresh, ghigh, glow

/* *********************************************************** */

    void mlib_s_ImageThresh1_F321(
	PARAMS_SSE2);
    void mlib_s_ImageThresh1_F322(
	PARAMS_SSE2);
    void mlib_s_ImageThresh1_F323(
	PARAMS_SSE2);
    void mlib_s_ImageThresh1_F324(
	PARAMS_SSE2);

    void mlib_s_ImageThresh1_D641(
	PARAMS_SSE2);
    void mlib_s_ImageThresh1_D642(
	PARAMS_SSE2);
    void mlib_s_ImageThresh1_D643(
	PARAMS_SSE2);
    void mlib_s_ImageThresh1_D644(
	PARAMS_SSE2);

	void mlib_c_ImageThresh1_D641_1B(
    PARAMS_BIT);
	void mlib_c_ImageThresh1_D642_1B(
    PARAMS_BIT);
	void mlib_c_ImageThresh1_D643_1B(
    PARAMS_BIT);
	void mlib_c_ImageThresh1_D644_1B(
    PARAMS_BIT);

	void mlib_c_ImageThresh1_F321_1B(
    PARAMS_BIT);
	void mlib_c_ImageThresh1_F322_1B(
    PARAMS_BIT);
	void mlib_c_ImageThresh1_F323_1B(
    PARAMS_BIT);
	void mlib_c_ImageThresh1_F324_1B(
    PARAMS_BIT);


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
	thresh0 = (DTYPE)thresh[(n)];                             \
	ghigh0 = (DTYPE)ghigh[(n)];                               \
	glow0 = (DTYPE)glow[(n)];

#define	INIT_THRESH1(n)                                       \
	thresh1 = (DTYPE)thresh[(n)];                             \
	ghigh1 = (DTYPE)ghigh[(n)];                               \
	glow1  = (DTYPE)glow[(n)];                                \

#define	INIT_THRESH2(n)                                       \
	thresh2 = (DTYPE)thresh[(n)];                             \
	ghigh2 = (DTYPE)ghigh[(n)];                               \
	glow2 = (DTYPE)glow[(n)];

#define	INIT_THRESH3(n)                                       \
	thresh3 = (DTYPE)thresh[(n)];                             \
	ghigh3 = (DTYPE)ghigh[(n)];                               \
	glow3  = (DTYPE)glow[(n)];                                \

/* *********************************************************** */

#define	SET_Pointers_PS(dst, src)                              \
	dst = (DTYPE *) ((mlib_addr)pdst_row);                     \
	src = (DTYPE *) ((mlib_addr)psrc_row);

/* *********************************************************** */

#define	DO_THRESH_F32(s0, th, gh, gl)                         \
	((s0) > (th)) ? (gh) : (gl)

#define	THRESH0_F32(s0)	DO_THRESH_F32(s0, thresh0, ghigh0, glow0)
#define	THRESH1_F32(s0)	DO_THRESH_F32(s0, thresh1, ghigh1, glow1)
#define	THRESH2_F32(s0)	DO_THRESH_F32(s0, thresh2, ghigh2, glow2)
#define	THRESH3_F32(s0)	DO_THRESH_F32(s0, thresh3, ghigh3, glow3)

/* *********************************************************** */
#define	DO_FAST_THRESH_F32(dst, src, th_SSE2, gh_SSE2, gl_SSE2) \
	_s_32x4 = _mm_loadu_ps(src);                                \
	mask_gt_th = _mm_cmpgt_ps(_s_32x4, th_SSE2);                \
	dd = _mm_or_ps(                                             \
		_mm_andnot_ps(mask_gt_th, gl_SSE2),                     \
	    _mm_and_ps(gh_SSE2, mask_gt_th));                       \
	_mm_storeu_ps(dst, dd);                                     \
	dst += 4;                                                   \
	src += 4
/* *********************************************************** */

#define	GROSS_LOOPS_F32                                         \
	for (j = 0; j < w3; j += 4) {                               \
	    DO_FAST_THRESH_F32(dataPtr, sourcePtr, _s_th,           \
		_s_gh, _s_gl);                                          \
	}
/* *********************************************************** */


void
mlib_s_ImageThresh1_F321(
	PARAMS_SSE2)
{
	mlib_s32 i, j, k, w3 = width - 3;
	const DTYPE *psrc_row = psrc;
	DTYPE thresh0, ghigh0, glow0;
	DTYPE *pdst_row = pdst;
	DTYPE *dataPtr, *sourcePtr;
	STYPE _s_th = _mm_set1_ps((DTYPE)thresh[0]);
	STYPE _s_gh = _mm_set1_ps((DTYPE)ghigh[0]);
	STYPE _s_gl = _mm_set1_ps((DTYPE)glow[0]);
	STYPE _s_32x4, mask_gt_th, dd;

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
mlib_s_ImageThresh1_F322(
	PARAMS_SSE2)
{
	mlib_s32 i, j, k, w3 = width;
	const DTYPE *psrc_row = psrc;
	DTYPE *pdst_row = pdst;
	DTYPE thresh0, thresh1;
	DTYPE ghigh0, ghigh1;
	DTYPE glow0, glow1;

	DTYPE *dataPtr, *sourcePtr;
	STYPE _s_th = _mm_setr_ps(
			(DTYPE)thresh[0], (DTYPE)thresh[1],
			(DTYPE)thresh[0], (DTYPE)thresh[1]);
	STYPE _s_gh = _mm_setr_ps(
			(DTYPE)ghigh[0], (DTYPE)ghigh[1],
			(DTYPE)ghigh[0], (DTYPE)ghigh[1]);
	STYPE _s_gl = _mm_setr_ps(
			(DTYPE)glow[0], (DTYPE)glow[1],
			(DTYPE)glow[0], (DTYPE)glow[1]);
	STYPE _s_32x4, mask_gt_th, dd;

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
mlib_s_ImageThresh1_F323(
	PARAMS_SSE2)
{
	mlib_s32 i, j, k;
	DTYPE thresh0, thresh1, thresh2;
	DTYPE ghigh0, ghigh1, ghigh2;
	DTYPE glow0, glow1, glow2;
	const DTYPE *psrc_row = psrc;
	DTYPE *pdst_row = pdst;
	DTYPE *dataPtr, *sourcePtr;

	STYPE _s_th0 = _mm_setr_ps(
		(DTYPE)thresh[0], (DTYPE)thresh[1],
		(DTYPE)thresh[2], (DTYPE)thresh[0]);
	STYPE _s_th1 = _mm_setr_ps(
		(DTYPE)thresh[1], (DTYPE)thresh[2],
		(DTYPE)thresh[0], (DTYPE)thresh[1]);
	STYPE _s_th2 = _mm_setr_ps(
		(DTYPE)thresh[2], (DTYPE)thresh[0],
		(DTYPE)thresh[1], (DTYPE)thresh[2]);
	STYPE _s_gh0 = _mm_setr_ps(
		(DTYPE)ghigh[0], (DTYPE)ghigh[1],
		(DTYPE)ghigh[2], (DTYPE)ghigh[0]);
	STYPE _s_gh1 = _mm_setr_ps(
		(DTYPE)ghigh[1], (DTYPE)ghigh[2],
		(DTYPE)ghigh[0], (DTYPE)ghigh[1]);
	STYPE _s_gh2 = _mm_setr_ps(
		(DTYPE)ghigh[2], (DTYPE)ghigh[0],
		(DTYPE)ghigh[1], (DTYPE)ghigh[2]);
	STYPE _s_gl0 = _mm_setr_ps(
		(DTYPE)glow[0], (DTYPE)glow[1],
		(DTYPE)glow[2], (DTYPE)glow[0]);
	STYPE _s_gl1 = _mm_setr_ps(
		(DTYPE)glow[1], (DTYPE)glow[2],
		(DTYPE)glow[0], (DTYPE)glow[1]);
	STYPE _s_gl2 = _mm_setr_ps(
		(DTYPE)glow[2], (DTYPE)glow[0],
		(DTYPE)glow[1], (DTYPE)glow[2]);
	STYPE _s_32x4, mask_gt_th, dd;

	INIT_THRESH0(0);
	INIT_THRESH1(1);
	INIT_THRESH2(2);
	width = width * 3;

	for (i = 0; i < height; i++) {

		SET_Pointers_PS(dataPtr, sourcePtr);

		for (j = 0; j < (width - 11); j += 12) {
			DO_FAST_THRESH_F32(dataPtr, sourcePtr,
			    _s_th0, _s_gh0, _s_gl0);
			DO_FAST_THRESH_F32(dataPtr, sourcePtr,
			    _s_th1, _s_gh1, _s_gl1);
			DO_FAST_THRESH_F32(dataPtr, sourcePtr,
			    _s_th2, _s_gh2, _s_gl2);
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
mlib_s_ImageThresh1_F324(
	PARAMS_SSE2)
{
	mlib_s32 i, j, k, w3;
	const DTYPE *psrc_row = psrc;
	DTYPE *pdst_row = pdst;

	DTYPE *dataPtr, *sourcePtr;
	STYPE _s_th = _mm_setr_ps(
			(DTYPE)thresh[0], (DTYPE)thresh[1],
			(DTYPE)thresh[2], (DTYPE)thresh[3]);
	STYPE _s_gh = _mm_setr_ps(
			(DTYPE)ghigh[0], (DTYPE)ghigh[1],
			(DTYPE)ghigh[2], (DTYPE)ghigh[3]);
	STYPE _s_gl = _mm_setr_ps(
			(DTYPE)glow[0], (DTYPE)glow[1],
			(DTYPE)glow[2], (DTYPE)glow[3]);
	STYPE _s_32x4, mask_gt_th, dd;

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
	thresh0 = (DTYPE)thresh[(n)];                             \
	ghigh0 = (DTYPE)ghigh[(n)];                               \
	glow0 = (DTYPE)glow[(n)];

#define	INIT_THRESH1(n)                                       \
	thresh1 = (DTYPE)thresh[(n)];                             \
	ghigh1 = (DTYPE)ghigh[(n)];                               \
	glow1  = (DTYPE)glow[(n)];                                \

#define	INIT_THRESH2(n)                                       \
	thresh2 = (DTYPE)thresh[(n)];                             \
	ghigh2 = (DTYPE)ghigh[(n)];                               \
	glow2 = (DTYPE)glow[(n)];

#define	INIT_THRESH3(n)                                       \
	thresh3 = (DTYPE)thresh[(n)];                             \
	ghigh3 = (DTYPE)ghigh[(n)];                               \
	glow3  = (DTYPE)glow[(n)];                                \

/* *********************************************************** */

#define	SET_Pointers_PD(dst, src)                             \
	dst = (DTYPE *) ((mlib_addr)pdst_row);                    \
	src = (DTYPE *) ((mlib_addr)psrc_row);

/* *********************************************************** */

#define	DO_THRESH_D64(s0, th, gh, gl)                         \
	((s0) > (th)) ? (gh) : (gl)

#define	THRESH0_D64(s0)	DO_THRESH_D64(s0, thresh0, ghigh0, glow0)
#define	THRESH1_D64(s0)	DO_THRESH_D64(s0, thresh1, ghigh1, glow1)
#define	THRESH2_D64(s0)	DO_THRESH_D64(s0, thresh2, ghigh2, glow2)
#define	THRESH3_D64(s0)	DO_THRESH_D64(s0, thresh3, ghigh3, glow3)

/* *********************************************************** */
#define	DO_FAST_THRESH_D64(dst, src, th_SSE2, gh_SSE2, gl_SSE2) \
	_s_64x2 = _mm_loadu_pd(src);                                \
	mask_gt_th = _mm_cmpgt_pd(_s_64x2, th_SSE2);                \
	dd = _mm_or_pd(                                             \
		_mm_andnot_pd(mask_gt_th, gl_SSE2),                     \
	    _mm_and_pd(gh_SSE2, mask_gt_th));                       \
	_mm_storeu_pd(dst, dd);                                     \
	dst += 2;                                                   \
	src += 2;
/* *********************************************************** */

#define	GROSS_LOOPS_D64                                         \
	for (j = 0; j < w1; j += 2) {                               \
	    DO_FAST_THRESH_D64(dataPtr, sourcePtr, _s_th,           \
		_s_gh, _s_gl);                                          \
	}
/* *********************************************************** */


void
mlib_s_ImageThresh1_D641(
	PARAMS_SSE2)
{
	mlib_s32 i, j, k, w1 = width - 1, tail = (width & 1);
	const DTYPE *psrc_row = psrc;
	DTYPE thresh0, ghigh0, glow0;
	DTYPE *pdst_row = pdst;
	DTYPE *dataPtr, *sourcePtr;
	STYPE _s_th = _mm_set1_pd((DTYPE)thresh[0]);
	STYPE _s_gh = _mm_set1_pd((DTYPE)ghigh[0]);
	STYPE _s_gl = _mm_set1_pd((DTYPE)glow[0]);
	STYPE _s_64x2, mask_gt_th, dd;

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
mlib_s_ImageThresh1_D642(
	PARAMS_SSE2)
{
	mlib_s32 i, j, k, w1;
	const DTYPE *psrc_row = psrc;
	DTYPE *pdst_row = pdst;

	DTYPE *dataPtr, *sourcePtr;
	STYPE _s_th = _mm_setr_pd(
			(DTYPE)thresh[0], (DTYPE)thresh[1]);
	STYPE _s_gh = _mm_setr_pd(
			(DTYPE)ghigh[0], (DTYPE)ghigh[1]);
	STYPE _s_gl = _mm_setr_pd(
			(DTYPE)glow[0], (DTYPE)glow[1]);
	STYPE _s_64x2, mask_gt_th, dd;

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
mlib_s_ImageThresh1_D643(
	PARAMS_SSE2)
{
	mlib_s32 i, j, k;
	DTYPE thresh0, thresh1, thresh2;
	DTYPE ghigh0, ghigh1, ghigh2;
	DTYPE glow0, glow1, glow2;
	const DTYPE *psrc_row = psrc;
	DTYPE *pdst_row = pdst;
	DTYPE *dataPtr, *sourcePtr;
	STYPE _s_th0 = _mm_setr_pd(
		(DTYPE)thresh[0], (DTYPE)thresh[1]);
	STYPE _s_th1 = _mm_setr_pd(
		(DTYPE)thresh[2], (DTYPE)thresh[0]);
	STYPE _s_th2 = _mm_setr_pd(
		(DTYPE)thresh[1], (DTYPE)thresh[2]);
	STYPE _s_gh0 = _mm_setr_pd(
		(DTYPE)ghigh[0], (DTYPE)ghigh[1]);
	STYPE _s_gh1 = _mm_setr_pd(
		(DTYPE)ghigh[2], (DTYPE)ghigh[0]);
	STYPE _s_gh2 = _mm_setr_pd(
		(DTYPE)ghigh[1], (DTYPE)ghigh[2]);
	STYPE _s_gl0 = _mm_setr_pd(
		(DTYPE)glow[0], (DTYPE)glow[1]);
	STYPE _s_gl1 = _mm_setr_pd(
		(DTYPE)glow[2], (DTYPE)glow[0]);
	STYPE _s_gl2 = _mm_setr_pd(
		(DTYPE)glow[1], (DTYPE)glow[2]);
	STYPE _s_64x2, mask_gt_th, dd;

	INIT_THRESH0(0);
	INIT_THRESH1(1);
	INIT_THRESH2(2);
	width = width * 3;

	for (i = 0; i < height; i++) {

		SET_Pointers_PD(dataPtr, sourcePtr);

		for (j = 0; j < (width - 5); j += 6) {
			DO_FAST_THRESH_D64(dataPtr, sourcePtr,
			    _s_th0, _s_gh0, _s_gl0);
			DO_FAST_THRESH_D64(dataPtr, sourcePtr,
			    _s_th1, _s_gh1, _s_gl1);
			DO_FAST_THRESH_D64(dataPtr, sourcePtr,
			    _s_th2, _s_gh2, _s_gl2);
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
mlib_s_ImageThresh1_D644(
	PARAMS_SSE2)
{
	mlib_s32 i, j, k;
	const DTYPE *psrc_row = psrc;
	DTYPE *pdst_row = pdst;

	DTYPE *dataPtr, *sourcePtr;
	STYPE _s_th0 = _mm_setr_pd(
			(DTYPE)thresh[0], (DTYPE)thresh[1]);
	STYPE _s_th1 = _mm_setr_pd(
			(DTYPE)thresh[2], (DTYPE)thresh[3]);
	STYPE _s_gh0 = _mm_setr_pd(
			(DTYPE)ghigh[0], (DTYPE)ghigh[1]);
	STYPE _s_gh1 = _mm_setr_pd(
			(DTYPE)ghigh[2], (DTYPE)ghigh[3]);
	STYPE _s_gl0 = _mm_setr_pd(
			(DTYPE)glow[0], (DTYPE)glow[1]);
	STYPE _s_gl1 = _mm_setr_pd(
			(DTYPE)glow[2], (DTYPE)glow[3]);
	STYPE _s_64x2, mask_gt_th, dd;

	width <<= 2;

	for (i = 0; i < height; i++) {

		SET_Pointers_PD(dataPtr, sourcePtr);
		for (j = 0; j < (width - 3); j += 4) {
			DO_FAST_THRESH_D64(dataPtr, sourcePtr,
			    _s_th0, _s_gh0, _s_gl0);
			DO_FAST_THRESH_D64(dataPtr, sourcePtr,
			    _s_th1, _s_gh1, _s_gl1);
		}

		psrc_row += src_stride;
		pdst_row += dst_stride;
	}
}

/* *********************************************************** */

typedef void (*mlib_func_type1) (PARAMS_SSE2);
typedef void (*mlib_func_type2) (PARAMS_BIT);

/* *********************************************************** */

static const mlib_func_type1 mlib_func_arr1[] = {
	mlib_s_ImageThresh1_F321,
	mlib_s_ImageThresh1_F322,
	mlib_s_ImageThresh1_F323,
	mlib_s_ImageThresh1_F324,
	mlib_s_ImageThresh1_D641,
	mlib_s_ImageThresh1_D642,
	mlib_s_ImageThresh1_D643,
	mlib_s_ImageThresh1_D644
};

static const mlib_func_type2 mlib_func_arr2[] = {
	mlib_c_ImageThresh1_F321_1B,
	mlib_c_ImageThresh1_F322_1B,
	mlib_c_ImageThresh1_F323_1B,
	mlib_c_ImageThresh1_F324_1B,
	mlib_c_ImageThresh1_D641_1B,
	mlib_c_ImageThresh1_D642_1B,
	mlib_c_ImageThresh1_D643_1B,
	mlib_c_ImageThresh1_D644_1B
};

/* *********************************************************** */

mlib_status
__mlib_ImageThresh1_Fp(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *thresh,
    const mlib_d64 *ghigh,
    const mlib_d64 *glow)
{
	mlib_s32 sstride, dstride, width, height, nchan;
	mlib_type stype, dtype;
	void *psrc, *pdst;

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

	if (stype == MLIB_FLOAT)
		sstride >>= 2;
	else if (stype == MLIB_DOUBLE)
		sstride >>= 3;
	else
		return (MLIB_FAILURE);

	if (dtype == MLIB_FLOAT)
		dstride >>= 2;
	if (dtype == MLIB_DOUBLE)
		dstride >>= 3;

	if (dtype != MLIB_BIT) {
		mlib_func_arr1[(stype - MLIB_FLOAT) * 4 + (nchan -
		    1)] (CALL_PARAMS_SSE2);
	} else {
		mlib_s32 dbit_off = mlib_ImageGetBitOffset(dst);

		mlib_func_arr2[(stype - MLIB_FLOAT) * 4 + (nchan -
		    1)] (CALL_PARAMS_BIT);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
