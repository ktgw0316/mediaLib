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
 * Copyright 2005 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_m_ImageFilteredSubsample_U8.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *
 *      mlib_ImageFilteredSubsample - Antialias filter and subsample an image
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageFilteredSubsample(mlib_image  *dst,
 *                                              const mlib_image  *src,
 *                                              mlib_s32    scaleX,
 *                                              mlib_s32    scaleY,
 *                                              mlib_s32    transX,
 *                                              mlib_s32    transY,
 *                                              const mlib_d64    *hKernel,
 *                                              const mlib_d64    *vKernel,
 *                                              mlib_s32    hSize,
 *                                              mlib_s32    vSize,
 *                                              mlib_s32    hParity,
 *                                              mlib_s32    vParity,
 *                                              mlib_edge   edge)
 *
 * ARGUMENTS
 *
 *      dst       Pointer to the destination image.
 *      src       Pointer to the source image.
 *      scaleX    The x scale factor of subsampling.
 *      scaleY    The y scale factor of subsampling.
 *      transX    The x translation.
 *      transY    The y translation.
 *      hKernel   Pointer to the compact form of the horizontal kernel.
 *      vKernel   Pointer to the compact form of the vertical kernel.
 *      hSize     Size of the array hKernel.
 *      vSize     Size of the array vKernel.
 *      hParity   Parity of the horizontal kernel (0 => even, 1 => odd).
 *      vParity   Parity of the vertical kernel (0 => even, 1 => odd).
 *      edge      Type of edge condition.
 *
 * DESCRIPTION
 *
 *      An operator to antialias filter and subsample images.  The effect of
 *      this operation on an image is equivalent to performing convolution
 *      (filter) followed by subsample (zoom out) operations.
 *
 *      This operator is similar to the image ZoomTranslate operator.  But
 *      they have different definitions on scale factors and translations,
 *      hence use different coordinate mapping equations.  The scaleX and
 *      scaleY used by FilteredSubsample are the reciprocals of the zoomx
 *      and zoomy, respectively, used by ZoomTranslate.
 *
 *      The FilteredSubsample operator mapping equations are given by
 *
 *          xS = xD*scaleX + transX
 *          yS = yD*scaleY + transY
 *
 *      where, a point (xD, yD) in the destination image is backward mapped
 *      to a point (xS, yS) in the source image.  The arguments transX and
 *      transY are provided to support tiling.
 *
 *      The subsample terms, i.e., the scale factors scaleX and scaleY,
 *      are restricted to positive integral values.  Geometrically, one
 *      destination pixel maps to scaleX by scaleY source pixels.
 *      With odd scale factors, destination pixel centers map directly onto
 *      source pixel centers.  With even scale factors, destination pixels
 *      map squarely between source pixel centers.  Below are examples of
 *      even, odd, and combination cases.
 *
 *          s = source pixel centers
 *          d = destination pixel centers mapped to source
 *
 *          s   s   s   s   s   s           s   s   s   s   s   s
 *            d       d       d
 *          s   s   s   s   s   s           s   d   s   s   d   s
 *
 *          s   s   s   s   s   s           s   s   s   s   s   s
 *            d       d       d
 *          s   s   s   s   s   s           s   s   s   s   s   s
 *
 *          s   s   s   s   s   s           s   d   s   s   d   s
 *            d       d       d
 *          s   s   s   s   s   s           s   s   s   s   s   s
 *
 *          Even scaleX/Y factors            Odd scaleX/Y factors
 *
 *          s   s   s   s   s   s           s   s   s   s   s   s
 *              d           d
 *          s   s   s   s   s   s           s d s   s d s   s d s
 *
 *          s   s   s   s   s   s           s   s   s   s   s   s
 *              d           d
 *          s   s   s   s   s   s           s   s   s   s   s   s
 *
 *          s   s   s   s   s   s           s d s   s d s   s d s
 *              d           d
 *          s   s   s   s   s   s           s   s   s   s   s   s
 *
 *         Odd/even scaleX/Y factors      Even/odd scaleX/Y factors
 *
 *      The applied filter is quadrant symmetric (typically antialias +
 *      resample).  The filter is product-separable, quadrant symmetric,
 *      and is defined by half of its span.  Parity is used to signify
 *      whether the symmetric kernel has a double center (even parity) or
 *      a single center value (odd parity).  For example, if hParity == 0
 *      (even), the horizontal kernel is defined as:
 *
 *      hKernel[hSize-1], ..., hKernel[0], hKernel[0], ..., hKernel[hSize-1]
 *
 *      Otherwise, if hParity == 1 (odd), the horizontal kernel is defined
 *      as:
 *
 *          hKernel[hSize-1], ..., hKernel[0], ..., hKernel[hSize-1]
 *
 *      Horizontal and vertical kernels representing convolved resample
 *      (i.e., the combined separable kernels) can be computed from a
 *      convolution filter (with odd parity), a resample filter, and because
 *      the subsample factors affect resample weights, the subsample scale
 *      factors.  It is the user's responsibility to provide meaningful
 *      combined kernels.
 *
 *      To compute the value of a pixel centered at point (xD, yD) in the
 *      destination image, apply the combined kernel to the source image by
 *      aligning the kernel's geometric center to the backward mapped point
 *      (xS, yS) in the source image.  When this is done in a separable
 *      manner, the centers of horizontal and vertical kernels should match
 *      the xS and yS values, respectively.
 *
 *      The combination of subsampling and filtering has performance
 *      benefits over sequential operator usage in part due to the symmetry
 *      constraints imposed by only allowing integer parameters for scaling
 *      and only allowing separable symmetric filters.
 *
 * RESTRICTION
 *
 *      The src and the dst must be the same type and have same number
 *      of channels (1, 2, 3, or 4).
 *      The images can be of type MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or
 *      MLIB_INT.
 *
 *      The image pixels are assumed to be centered at .5 coordinate points,
 *      and the upper-left corner pixel of an image is located at (0.5, 0.5).
 *
 *      scaleX > 0
 *      scaleY > 0
 *
 *      The edge condition can be one of the following:
 *              MLIB_EDGE_DST_NO_WRITE  (default)
 */

/* #define MMX_USE_MODIF_8x16 */
#include <stdlib.h>
#include <mlib_mmx_utils.h>
#include <mlib_image.h>
#include <mlib_ImageCheck.h>

#define	IMG_TYPE	1

#define	KBUFF_SIZE	64

/* *********************************************************** */

#define	DSCALE	(1u << (31 - iscale))

#define	STYPE	mlib_u8

#define	FS_FUNC(SUFF)	mlib_m_ImageFilteredSubsample_8nw_##SUFF

/* *********************************************************** */

#define	PARAMS                                                            \
	__m64 *buff, STYPE * sp0, STYPE * sp1, __m64 * pkk,               \
	    mlib_s32 xsize, mlib_s32 off, mlib_s32 step, mlib_s32 nchan

/* *********************************************************** */

#define	PARAMS1	PARAMS, mlib_s32 iscale, STYPE *dp

/* *********************************************************** */

#define	LD_1x4_U8(s0, sp0)                                             \
	s0 =                                                           \
	_m_from_int(sp0[0] | (sp0[step] << 8) | (sp0[step2] << 16) |   \
	    (sp0[step3] << 24))

/* *********************************************************** */

#define	LD_1x2_S16(s0, sp0)                                     \
	s0 =                                                    \
	_m_from_int(*(mlib_u16 *)sp0 | ((*(mlib_u16 *)(sp0 +    \
	    step)) << 16))

/* *********************************************************** */

#define	LD_2x4_U8(s0, s1, sp0, sp1)                             \
	LD_1x4_U8(s0, sp0);                                     \
	LD_1x4_U8(s1, sp1)

/* *********************************************************** */

#define	STORE_4xU8(dp, dd)                                      \
	dd = _mm_packs_pu16(_mm_srai_pi16(dd, sh), _m_zero);    \
	di = _m_to_int(dd);                                     \
	*(mlib_s32 *)dp = di

/* *********************************************************** */

#define	STORE_EDGE(dp, dd, N)                                   \
	dd = _mm_packs_pu16(_mm_srai_pi16(dd, sh), _m_zero);    \
	di = _m_to_int(dd);                                     \
	dp[0] = di;                                             \
	if ((N) > 1)                                            \
	    dp[1] = di >> 8;                                    \
	if ((N) > 2)                                            \
	    dp[2] = di >> 16

/* *********************************************************** */

static void FS_FUNC(
    2x2i_1) (
    PARAMS)
{
	__m64 s0, s1, s2, s3, dd;
	__m64 kk = pkk[0];
	mlib_s32 i, step2, step3;

	step2 = step + step;
	step3 = step + step2;

	for (i = 0; i < (xsize + 3) / 4; i++) {
		mlib_u8 *sp0a = sp0 + off;
		mlib_u8 *sp1a = sp1 + off;

		LD_2x4_U8(s0, s1, sp0, sp0a);
		LD_2x4_U8(s2, s3, sp1, sp1a);

		dd = buff[i];
		dd = _mm_add_pi16(dd, mmx_fmul8x16hi(s0, kk));
		dd = _mm_add_pi16(dd, mmx_fmul8x16hi(s1, kk));
		dd = _mm_add_pi16(dd, mmx_fmul8x16hi(s2, kk));
		dd = _mm_add_pi16(dd, mmx_fmul8x16hi(s3, kk));
		buff[i] = dd;

		sp0 += 4 * step;
		sp1 += 4 * step;
	}

	_mm_empty();
}

/* *********************************************************** */

static void FS_FUNC(
    2x2l_1) (
    PARAMS1)
{
	__m64 s0, s1, s2, s3, dd;
	__m64 val0 = pkk[4], kk = pkk[0];
	mlib_s32 i, step2, step3;
	mlib_s32 di, sh = 7 - iscale;

	step2 = step + step;
	step3 = step + step2;

	for (i = 0; i < xsize / 4; i++) {
		mlib_u8 *sp0a = sp0 + off;
		mlib_u8 *sp1a = sp1 + off;

		LD_2x4_U8(s0, s1, sp0, sp0a);
		LD_2x4_U8(s2, s3, sp1, sp1a);

		dd = buff[i];
		dd = _mm_add_pi16(dd, mmx_fmul8x16hi(s0, kk));
		dd = _mm_add_pi16(dd, mmx_fmul8x16hi(s1, kk));
		dd = _mm_add_pi16(dd, mmx_fmul8x16hi(s2, kk));
		dd = _mm_add_pi16(dd, mmx_fmul8x16hi(s3, kk));
		buff[i] = val0;

		STORE_4xU8(dp, dd);

		sp0 += 4 * step;
		sp1 += 4 * step;
		dp += 4;
	}

	if (xsize & 3) {
		mlib_u8 *sp0a = sp0 + off;
		mlib_u8 *sp1a = sp1 + off;

		LD_2x4_U8(s0, s1, sp0, sp0a);
		LD_2x4_U8(s2, s3, sp1, sp1a);

		dd = buff[i];
		dd = _mm_add_pi16(dd, mmx_fmul8x16hi(s0, kk));
		dd = _mm_add_pi16(dd, mmx_fmul8x16hi(s1, kk));
		dd = _mm_add_pi16(dd, mmx_fmul8x16hi(s2, kk));
		dd = _mm_add_pi16(dd, mmx_fmul8x16hi(s3, kk));
		buff[i] = val0;

		STORE_EDGE(dp, dd, xsize & 3);
	}

	_mm_empty();
}

/* *********************************************************** */

static void FS_FUNC(
    1x1l_1) (
    PARAMS1)
{
	__m64 s0, dd;
	__m64 val0 = pkk[4], kk = pkk[0];
	mlib_s32 i, step2, step3;
	mlib_s32 di, sh = 7 - iscale;

	step2 = step + step;
	step3 = step + step2;

	for (i = 0; i < xsize / 4; i++) {
		LD_1x4_U8(s0, sp0);

		dd = buff[i];
		dd = _mm_add_pi16(dd, mmx_fmul8x16hi(s0, kk));
		buff[i] = val0;

		STORE_4xU8(dp, dd);

		sp0 += 4 * step;
		dp += 4;
	}

	if (xsize & 3) {
		LD_1x4_U8(s0, sp0);

		dd = buff[i];
		dd = _mm_add_pi16(dd, mmx_fmul8x16hi(s0, kk));
		buff[i] = val0;

		STORE_EDGE(dp, dd, xsize & 3);
	}

	_mm_empty();
}

/* *********************************************************** */

static void FS_FUNC(
    2x2i_2) (
    PARAMS)
{
	__m64 s0, s1, s2, s3, dd;
	__m64 kk = pkk[0];
	mlib_s32 i;

	for (i = 0; i < (xsize + 1) / 2; i++) {
		mlib_u8 *sp0a = sp0 + off;
		mlib_u8 *sp1a = sp1 + off;

		LD_1x2_S16(s0, sp0);
		LD_1x2_S16(s1, sp0a);
		LD_1x2_S16(s2, sp1);
		LD_1x2_S16(s3, sp1a);

		dd = buff[i];
		dd = _mm_add_pi16(dd, mmx_fmul8x16hi(s0, kk));
		dd = _mm_add_pi16(dd, mmx_fmul8x16hi(s1, kk));
		dd = _mm_add_pi16(dd, mmx_fmul8x16hi(s2, kk));
		dd = _mm_add_pi16(dd, mmx_fmul8x16hi(s3, kk));
		buff[i] = dd;

		sp0 += 2 * step;
		sp1 += 2 * step;
	}

	_mm_empty();
}

/* *********************************************************** */

static void FS_FUNC(
    2x2l_2) (
    PARAMS1)
{
	__m64 s0, s1, s2, s3, dd;
	__m64 val0 = pkk[4], kk = pkk[0];
	mlib_s32 i;
	mlib_s32 sh = 7 - iscale;

	for (i = 0; i < xsize / 2; i++) {
		mlib_u8 *sp0a = sp0 + off;
		mlib_u8 *sp1a = sp1 + off;

		LD_1x2_S16(s0, sp0);
		LD_1x2_S16(s1, sp0a);
		LD_1x2_S16(s2, sp1);
		LD_1x2_S16(s3, sp1a);

		dd = buff[i];
		dd = _mm_add_pi16(dd, mmx_fmul8x16hi(s0, kk));
		dd = _mm_add_pi16(dd, mmx_fmul8x16hi(s1, kk));
		dd = _mm_add_pi16(dd, mmx_fmul8x16hi(s2, kk));
		dd = _mm_add_pi16(dd, mmx_fmul8x16hi(s3, kk));
		buff[i] = val0;

		dd = _mm_packs_pu16(_mm_srai_pi16(dd, sh), _m_zero);
		*(mlib_s32 *)dp = _m_to_int(dd);

		sp0 += 2 * step;
		sp1 += 2 * step;
		dp += 4;
	}

	if (xsize & 1) {
		mlib_u8 *sp0a = sp0 + off;
		mlib_u8 *sp1a = sp1 + off;

		LD_1x2_S16(s0, sp0);
		LD_1x2_S16(s1, sp0a);
		LD_1x2_S16(s2, sp1);
		LD_1x2_S16(s3, sp1a);

		dd = buff[i];
		dd = _mm_add_pi16(dd, mmx_fmul8x16hi(s0, kk));
		dd = _mm_add_pi16(dd, mmx_fmul8x16hi(s1, kk));
		dd = _mm_add_pi16(dd, mmx_fmul8x16hi(s2, kk));
		dd = _mm_add_pi16(dd, mmx_fmul8x16hi(s3, kk));
		buff[i] = val0;

		dd = _mm_packs_pu16(_mm_srai_pi16(dd, sh), _m_zero);
		*(mlib_s16 *)dp = _m_to_int(dd);
	}

	_mm_empty();
}

/* *********************************************************** */

static void FS_FUNC(
    1x1l_2) (
    PARAMS1)
{
	__m64 s0, dd;
	__m64 val0 = pkk[4], kk = pkk[0];
	mlib_s32 i, step2;
	mlib_s32 sh = 7 - iscale;

	step2 = step + step;

	for (i = 0; i < xsize / 2; i++) {
		LD_1x2_S16(s0, sp0);

		dd = buff[i];
		dd = _mm_add_pi16(dd, mmx_fmul8x16hi(s0, kk));
		buff[i] = val0;

		dd = _mm_packs_pu16(_mm_srai_pi16(dd, sh), _m_zero);
		*(mlib_s32 *)dp = _m_to_int(dd);

		sp0 += 2 * step;
		dp += 4;
	}

	if (xsize & 1) {
		LD_1x2_S16(s0, sp0);

		dd = buff[i];
		dd = _mm_add_pi16(dd, mmx_fmul8x16hi(s0, kk));
		buff[i] = val0;

		dd = _mm_packs_pu16(_mm_srai_pi16(dd, sh), _m_zero);
		*(mlib_s16 *)dp = _m_to_int(dd);
	}
	_mm_empty();
}

/* *********************************************************** */

static void FS_FUNC(
    2x2i_4) (
    PARAMS)
{
	__m64 dd;
	__m64 kk = pkk[0];
	mlib_s32 i;

	for (i = 0; i < xsize; i++) {
		mlib_u8 *sp0a = sp0 + off;
		mlib_u8 *sp1a = sp1 + off;

		dd = buff[i];
		dd = _mm_add_pi16(dd, mmx_fmul8x16(*(mlib_s32 *)sp0, kk));
		dd = _mm_add_pi16(dd, mmx_fmul8x16(*(mlib_s32 *)sp0a, kk));
		dd = _mm_add_pi16(dd, mmx_fmul8x16(*(mlib_s32 *)sp1, kk));
		dd = _mm_add_pi16(dd, mmx_fmul8x16(*(mlib_s32 *)sp1a, kk));
		buff[i] = dd;

		sp0 += step;
		sp1 += step;
	}

	_mm_empty();
}

/* *********************************************************** */

static void FS_FUNC(
    2x2l_4) (
    PARAMS1)
{
	__m64 dd;
	__m64 val0 = pkk[4], kk = pkk[0];
	mlib_s32 sh = 7 - iscale;
	mlib_s32 i, di;

	if (nchan == 3)
		xsize--;

	for (i = 0; i < xsize; i++) {
		mlib_u8 *sp0a = sp0 + off;
		mlib_u8 *sp1a = sp1 + off;

		dd = buff[i];
		dd = _mm_add_pi16(dd, mmx_fmul8x16(*(mlib_s32 *)sp0, kk));
		dd = _mm_add_pi16(dd, mmx_fmul8x16(*(mlib_s32 *)sp0a, kk));
		dd = _mm_add_pi16(dd, mmx_fmul8x16(*(mlib_s32 *)sp1, kk));
		dd = _mm_add_pi16(dd, mmx_fmul8x16(*(mlib_s32 *)sp1a, kk));
		buff[i] = val0;

		dd = _mm_packs_pu16(_mm_srai_pi16(dd, sh), _m_zero);
		*(mlib_s32 *)dp = _m_to_int(dd);

		sp0 += step;
		sp1 += step;
		dp += nchan;
	}

	if (nchan == 3) {
		mlib_u8 *sp0a = sp0 + off;
		mlib_u8 *sp1a = sp1 + off;

		dd = buff[i];
		dd = _mm_add_pi16(dd, mmx_fmul8x16(*(mlib_s32 *)sp0, kk));
		dd = _mm_add_pi16(dd, mmx_fmul8x16(*(mlib_s32 *)sp0a, kk));
		dd = _mm_add_pi16(dd, mmx_fmul8x16(*(mlib_s32 *)sp1, kk));
		dd = _mm_add_pi16(dd, mmx_fmul8x16(*(mlib_s32 *)sp1a, kk));
		buff[i] = val0;

		STORE_EDGE(dp, dd, 3);
	}

	_mm_empty();
}

/* *********************************************************** */

static void FS_FUNC(
    1x1l_4) (
    PARAMS1)
{
	__m64 dd;
	__m64 val0 = pkk[4], kk = pkk[0];
	mlib_s32 sh = 7 - iscale;
	mlib_s32 i, di;

	if (nchan == 3)
		xsize--;

	for (i = 0; i < xsize; i++) {
		dd = buff[i];
		dd = _mm_add_pi16(dd, mmx_fmul8x16(*(mlib_s32 *)sp0, kk));
		buff[i] = val0;

		dd = _mm_packs_pu16(_mm_srai_pi16(dd, sh), _m_zero);
		*(mlib_s32 *)dp = _m_to_int(dd);

		sp0 += step;
		dp += nchan;
	}

	if (nchan == 3) {
		dd = buff[i];
		dd = _mm_add_pi16(dd, mmx_fmul8x16(*(mlib_s32 *)sp0, kk));
		buff[i] = val0;

		STORE_EDGE(dp, dd, 3);
	}

	_mm_empty();
}

/* *********************************************************** */

mlib_status FS_FUNC(
    1)  (
    mlib_image *dst,
    mlib_image *src,
    mlib_s32 scaleX,
    mlib_s32 scaleY,
    mlib_s32 transX,
    mlib_s32 transY,
    mlib_d64 *hKernel,
    mlib_d64 *vKernel,
    mlib_s32 iscale,
    mlib_s32 hSize,
    mlib_s32 vSize,
    mlib_s32 hParity,
    mlib_s32 vParity,
    mlib_edge edge)
{
	STYPE *psrc, *pdst, *sl, *dl;
	mlib_s32 slb, dlb, swidth, sheight, dwidth, dheight, nchan;
	mlib_type type;
	mlib_s32 kw = 2 * hSize - hParity;
	mlib_s32 kh = 2 * vSize - vParity;
	mlib_s32 step, stepRight, stepDown;
	mlib_s32 rnd_val;
	__m64 *buff, kk[5], val0;
	__m64 keri_lcl[KBUFF_SIZE], *keri = keri_lcl;
	mlib_s32 x_min, x_max, y_min, y_max, xsize, bsize;
	mlib_s32 i, j, k, ix, iy;

	MLIB_IMAGE_GET_ALL_PARAMS(dst, type, nchan, dwidth, dheight, dlb, pdst);
	MLIB_IMAGE_GET_ALL_PARAMS(src, type, nchan, swidth, sheight, slb, psrc);

	slb /= sizeof (STYPE);
	dlb /= sizeof (STYPE);

	step = scaleX * nchan;
	stepRight = (kw - 1) * nchan;
	stepDown = (kh - 1) * slb;

/* 0.5*scaleX - 0.5*(kw-1) */
	transX += (scaleX - (kw - 1)) >> 1;
/* 0.5*scaleY - 0.5*(kh-1) */
	transY += (scaleY - (kh - 1)) >> 1;

	x_min = scaleX - 1 - transX;
	y_min = scaleY - 1 - transY;
	x_max = swidth - kw - transX;
	y_max = sheight - kh - transY;

	if (x_min >= 0)
		x_min /= scaleX;
	else
		x_min = 0;
	if (y_min >= 0)
		y_min /= scaleY;
	else
		y_min = 0;
	if (x_max >= 0)
		x_max /= scaleX;
	else
		x_max = -1;
	if (y_max >= 0)
		y_max /= scaleY;
	else
		y_max = -1;
	if (x_max >= dwidth)
		x_max = dwidth - 1;
	if (y_max >= dheight)
		y_max = dheight - 1;

	xsize = x_max - x_min + 1;
	if (xsize <= 0)
		return (MLIB_SUCCESS);

	bsize =
	    (nchan == 1) ? ((xsize + 3) / 4) : (nchan ==
	    2) ? ((xsize + 1) / 2) : xsize;
	buff = mlib_malloc(sizeof (__m64) * bsize);
	if (buff == NULL)
		return (MLIB_FAILURE);

	if (vSize * hSize > KBUFF_SIZE) {
		keri = mlib_malloc(vSize * hSize * sizeof (__m64));
		if (keri == NULL) {
			mlib_free(buff);
			return (MLIB_FAILURE);
		}
	}

	for (iy = 0; iy < vSize; iy++) {
		mlib_s32 flagy = (iy == 0 && vParity) ? 1 : 0;

		for (ix = 0; ix < hSize; ix++) {
			mlib_s32 ki, flagx = (ix == 0 && hParity) ? 1 : 0;
			mlib_d64 kd;

			kd = hKernel[ix] * vKernel[iy] * DSCALE;
			if (flagx ^ flagy)
				kd *= 0.5;
			ki = (kd >
			    0) ? (mlib_s32)(kd + 0.5) : (mlib_s32)(kd - 0.5);
			ki = (ki & 0xFFFF0000) | ((ki >> 16) & 0xFFFF);
			((mlib_s32 *)(keri + iy * hSize + ix))[0] = ki;
			((mlib_s32 *)(keri + iy * hSize + ix))[1] = ki;
			ki &= 0xFFFF;
		}
	}

	rnd_val = 1 << (6 - iscale);
	val0 = mmx_to_double_dup(rnd_val | (rnd_val << 16));
	kk[4] = val0;
	for (i = 0; i < bsize; i++) {
		buff[i] = val0;
	}

	sl = psrc + (y_min * scaleY + transY) * slb + (x_min * scaleX +
	    transX) * nchan;
	dl = pdst + y_min * dlb + x_min * nchan;

	for (j = y_min; j <= y_max; j++) {
		STYPE *sl0 = sl;
		STYPE *sl1 = sl + stepDown;

		for (iy = vSize - 1; iy >= 0; iy--) {
			mlib_s32 off = stepRight;
			STYPE *sp0 = sl0;
			STYPE *sp1 = sl1;

			for (ix = hSize - 1; ix >= 0; ix--) {
				mlib_s32 flagl = (iy == 0 && ix == 0);

				kk[0] = keri[iy * hSize + ix];

				if (!flagl) {
					if (nchan == 1)
						FS_FUNC(2x2i_1) (buff, sp0,
						    sp1, kk, xsize, off, step,
						    nchan);
					else if (nchan == 2)
						FS_FUNC(2x2i_2) (buff, sp0,
						    sp1, kk, xsize, off, step,
						    nchan);
					else
						FS_FUNC(2x2i_4) (buff, sp0,
						    sp1, kk, xsize, off, step,
						    nchan);
				} else if (off == 0 && sp0 == sp1) {
					if (nchan == 1)
						FS_FUNC(1x1l_1) (buff, sp0,
						    sp1, kk, xsize, off, step,
						    nchan, iscale, dl + k);
					else if (nchan == 2)
						FS_FUNC(1x1l_2) (buff, sp0,
						    sp1, kk, xsize, off, step,
						    nchan, iscale, dl + k);
					else
						FS_FUNC(1x1l_4) (buff, sp0,
						    sp1, kk, xsize, off, step,
						    nchan, iscale, dl + k);
				} else {
					if (nchan == 1)
						FS_FUNC(2x2l_1) (buff, sp0,
						    sp1, kk, xsize, off, step,
						    nchan, iscale, dl + k);
					else if (nchan == 2)
						FS_FUNC(2x2l_2) (buff, sp0,
						    sp1, kk, xsize, off, step,
						    nchan, iscale, dl + k);
					else
						FS_FUNC(2x2l_4) (buff, sp0,
						    sp1, kk, xsize, off, step,
						    nchan, iscale, dl + k);
				}

				sp0 += nchan;
				sp1 += nchan;
				off -= 2 * nchan;
			}

			sl0 += slb;
			sl1 -= slb;
		}

		sl += scaleY * slb;
		dl += dlb;
	}

	if (keri != keri_lcl)
		mlib_free(keri);
	mlib_free(buff);

	_mm_empty();
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
