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

#pragma ident	"@(#)mlib_m_ImageAffineTable_2ch.c	9.4	07/11/05 SMI"

/*
 * FUNCTION
 *      Internal functions for mlib_ImageAffineTable.
 */

#define	MMX_USE_MEM_ZERO

#define	MMX_USE_MODIF_8x16

#include <stdlib.h>
#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <mmintrin.h>
#endif

#include <mlib_mmx_utils.h>
#include <mlib_image.h>
#include <mlib_ImageInterpTable.h>
#include <mlib_ImageAffineTable.h>

/* ******************************************************************** */
#define	DECLAREVAR                                              \
	mlib_u8 *sPtr;                                          \
	mlib_s32 *dstPixelPtr;                                  \
	mlib_s32 srcStride = ws->srcStride;                     \
	mlib_s32 i, filterpos, xSrc, ySrc;                      \
	mlib_s32 dx = ws->dx;                                   \
	mlib_s32 dy = ws->dy;                                   \
	mlib_s32 x_shift = ws->x_shift;                         \
	mlib_s32 y_shift = ws->y_shift;                         \
	mlib_s32 xf_shift = ws->xf_shift;                       \
	mlib_s32 xf_mask = ws->xf_mask;                         \
	mlib_s32 yf_shift = ws->yf_shift;                       \
	mlib_s32 yf_mask = ws->yf_mask;                         \
	mlib_s32 size = ws->size;                               \
	mlib_s32 x = ws->x0;                                    \
	mlib_s32 y = ws->y0;                                    \
	__m64 *fptrx, *fptry;
/* ******************************************************************** */
#define	CALC_PTR                                                      \
	filterpos = (x >> xf_shift) & xf_mask;                        \
	fptrx = ((__m64 *) ((mlib_u8 *)filterX + 2 * filterpos));     \
	filterpos = (y >> yf_shift) & yf_mask;                        \
	fptry = (__m64 *) ((mlib_u8 *)filterY + 4 * filterpos);       \
	xSrc = (x >> x_shift) & ~1;                                   \
	ySrc = (y >> y_shift);                                        \
	sPtr = (mlib_u8 *)(((MLIB_TYPE **) lineAddr)[ySrc] + xSrc);
/* ******************************************************************** */

#define	MLIB_TYPE	mlib_u8

/* ******************************************************************** */

void
mlib_m_ImageAffineTableLine_8nw_4_4_2(
    __m64 * buff,
    __m64 * filterX,
    __m64 * filterY,
    mlib_u8 **lineAddr,
    mlib_affine_workspace * ws)
{
	DECLAREVAR __m64 _m_mzero[1] = {
	0};
	__m64 yf0, yf1, yf2, yf3;
	__m64 a0, a1, a2, a3;
	__m64 b0, b1, b2, b3;

	dstPixelPtr = (mlib_s32 *)buff;

	for (i = 0; i < size; i++) {
		CALC_PTR yf0 = fptry[0];
		yf1 = fptry[1];
		yf2 = fptry[2];
		yf3 = fptry[3];

		a0 = mmx_fmul8x16hi(*(__m64 *) (sPtr), yf0);
		a1 = mmx_fmul8x16hi(*(__m64 *) (sPtr + srcStride), yf1);
		a2 = mmx_fmul8x16hi(*(__m64 *) (sPtr + 2 * srcStride), yf2);
		a3 = mmx_fmul8x16hi(*(__m64 *) (sPtr + 3 * srcStride), yf3);
		b0 = mmx_fmul8x16hi(*(__m64 *) (sPtr + 4), yf0);
		b1 = mmx_fmul8x16hi(*(__m64 *) (sPtr + srcStride + 4), yf1);
		b2 = mmx_fmul8x16hi(*(__m64 *) (sPtr + 2 * srcStride + 4), yf2);
		b3 = mmx_fmul8x16hi(*(__m64 *) (sPtr + 3 * srcStride + 4), yf3);
		a0 = _mm_add_pi16(a0, a1);
		a2 = _mm_add_pi16(a2, a3);
		a0 = _mm_add_pi16(a0, a2);
		b0 = _mm_add_pi16(b0, b1);
		b2 = _mm_add_pi16(b2, b3);
		b0 = _mm_add_pi16(b0, b2);
		a0 = _mm_mulhi_pi16(a0, fptrx[0]);
		b0 = _mm_mulhi_pi16(b0, fptrx[1]);
		a0 = _mm_add_pi16(a0, b0);
		a0 = _mm_add_pi16(a0, _mm_srli_si64(a0, 32));

		*dstPixelPtr = _m_to_int(a0);
		dstPixelPtr++;
		x += dx;
		y += dy;
	}

	_mm_empty();
}

/* ******************************************************************** */

void
mlib_m_ImageAffineTableLine_8nw_3_4_2(
    __m64 * buff,
    __m64 * filterX,
    __m64 * filterY,
    mlib_u8 **lineAddr,
    mlib_affine_workspace * ws)
{
	DECLAREVAR __m64 _m_mzero[1] = {
	0};
	__m64 yf0, yf1, yf2;
	__m64 a0, a1, a2;
	__m64 b0, b1, b2;

	dstPixelPtr = (mlib_s32 *)buff;

	for (i = 0; i < size; i++) {
		CALC_PTR yf0 = fptry[0];
		yf1 = fptry[1];
		yf2 = fptry[2];

		a0 = mmx_fmul8x16hi(*(__m64 *) (sPtr), yf0);
		a1 = mmx_fmul8x16hi(*(__m64 *) (sPtr + srcStride), yf1);
		a2 = mmx_fmul8x16hi(*(__m64 *) (sPtr + 2 * srcStride), yf2);
		b0 = mmx_fmul8x16hi(*(__m64 *) (sPtr + 4), yf0);
		b1 = mmx_fmul8x16hi(*(__m64 *) (sPtr + srcStride + 4), yf1);
		b2 = mmx_fmul8x16hi(*(__m64 *) (sPtr + 2 * srcStride + 4), yf2);
		a0 = _mm_add_pi16(a0, a1);
		a0 = _mm_add_pi16(a0, a2);
		b0 = _mm_add_pi16(b0, b1);
		b0 = _mm_add_pi16(b0, b2);
		a0 = _mm_mulhi_pi16(a0, fptrx[0]);
		b0 = _mm_mulhi_pi16(b0, fptrx[1]);
		a0 = _mm_add_pi16(a0, b0);
		a0 = _mm_add_pi16(a0, _mm_srli_si64(a0, 32));

		*dstPixelPtr = _m_to_int(a0);
		dstPixelPtr++;
		x += dx;
		y += dy;
	}

	_mm_empty();
}

/* ******************************************************************** */

void
mlib_m_ImageAffineTableLine_8nw_2_4_2(
    __m64 * buff,
    __m64 * filterX,
    __m64 * filterY,
    mlib_u8 **lineAddr,
    mlib_affine_workspace * ws)
{
	DECLAREVAR __m64 _m_mzero[1] = {
	0};
	__m64 yf0, yf1, a0, a1;
	__m64 b0, b1;

	dstPixelPtr = (mlib_s32 *)buff;

	for (i = 0; i < size; i++) {
		CALC_PTR yf0 = fptry[0];
		yf1 = fptry[1];

		a0 = mmx_fmul8x16hi(*(__m64 *) (sPtr), yf0);
		a1 = mmx_fmul8x16hi(*(__m64 *) (sPtr + srcStride), yf1);
		b0 = mmx_fmul8x16hi(*(__m64 *) (sPtr + 4), yf0);
		b1 = mmx_fmul8x16hi(*(__m64 *) (sPtr + srcStride + 4), yf1);
		a0 = _mm_add_pi16(a0, a1);
		b0 = _mm_add_pi16(b0, b1);
		a0 = _mm_mulhi_pi16(a0, fptrx[0]);
		b0 = _mm_mulhi_pi16(b0, fptrx[1]);
		a0 = _mm_add_pi16(a0, b0);
		a0 = _mm_add_pi16(a0, _mm_srli_si64(a0, 32));

		*dstPixelPtr = _m_to_int(a0);
		dstPixelPtr++;
		x += dx;
		y += dy;
	}

	_mm_empty();
}

/* ******************************************************************** */

void
mlib_m_ImageAffineTableLine_8nw_4_2_2(
    __m64 * buff,
    __m64 * filterX,
    __m64 * filterY,
    mlib_u8 **lineAddr,
    mlib_affine_workspace * ws)
{
	DECLAREVAR __m64 _m_mzero[1] = {
	0};
	__m64 yf0, yf1, yf2, yf3;
	__m64 a0, a1, a2, a3;

	dstPixelPtr = (mlib_s32 *)buff;

	for (i = 0; i < size; i++) {
		CALC_PTR yf0 = fptry[0];
		yf1 = fptry[1];
		yf2 = fptry[2];
		yf3 = fptry[3];

		a0 = mmx_fmul8x16hi(*(__m64 *) (sPtr), yf0);
		a1 = mmx_fmul8x16hi(*(__m64 *) (sPtr + srcStride), yf1);
		a2 = mmx_fmul8x16hi(*(__m64 *) (sPtr + 2 * srcStride), yf2);
		a3 = mmx_fmul8x16hi(*(__m64 *) (sPtr + 3 * srcStride), yf3);
		a0 = _mm_add_pi16(a0, a1);
		a2 = _mm_add_pi16(a2, a3);
		a0 = _mm_add_pi16(a0, a2);
		a0 = _mm_mulhi_pi16(a0, fptrx[0]);
		a0 = _mm_add_pi16(a0, _mm_srli_si64(a0, 32));

		*dstPixelPtr = _m_to_int(a0);
		dstPixelPtr++;
		x += dx;
		y += dy;
	}

	_mm_empty();
}

/* ******************************************************************** */

void
mlib_m_ImageAffineTableLine_8nw_3_2_2(
    __m64 * buff,
    __m64 * filterX,
    __m64 * filterY,
    mlib_u8 **lineAddr,
    mlib_affine_workspace * ws)
{
	DECLAREVAR __m64 _m_mzero[1] = {
	0};
	__m64 yf0, yf1, yf2;
	__m64 a0, a1, a2;

	dstPixelPtr = (mlib_s32 *)buff;

	for (i = 0; i < size; i++) {
		CALC_PTR yf0 = fptry[0];
		yf1 = fptry[1];
		yf2 = fptry[2];

		a0 = mmx_fmul8x16hi(*(__m64 *) (sPtr), yf0);
		a1 = mmx_fmul8x16hi(*(__m64 *) (sPtr + srcStride), yf1);
		a2 = mmx_fmul8x16hi(*(__m64 *) (sPtr + 2 * srcStride), yf2);
		a0 = _mm_add_pi16(a0, a1);
		a0 = _mm_add_pi16(a0, a2);
		a0 = _mm_mulhi_pi16(a0, fptrx[0]);
		a0 = _mm_add_pi16(a0, _mm_srli_si64(a0, 32));

		*dstPixelPtr = _m_to_int(a0);
		dstPixelPtr++;
		x += dx;
		y += dy;
	}

	_mm_empty();
}

/* ******************************************************************** */

void
mlib_m_ImageAffineTableLine_8nw_2_2_2(
    __m64 * buff,
    __m64 * filterX,
    __m64 * filterY,
    mlib_u8 **lineAddr,
    mlib_affine_workspace * ws)
{
	DECLAREVAR __m64 _m_mzero[1] = {
	0};
	__m64 yf0, yf1, a0, a1;

	dstPixelPtr = (mlib_s32 *)buff;

	for (i = 0; i < size; i++) {
		CALC_PTR yf0 = fptry[0];
		yf1 = fptry[1];

		a0 = mmx_fmul8x16hi(*(__m64 *) (sPtr), yf0);
		a1 = mmx_fmul8x16hi(*(__m64 *) (sPtr + srcStride), yf1);
		a0 = _mm_add_pi16(a0, a1);

		a0 = _mm_mulhi_pi16(a0, fptrx[0]);
		a0 = _mm_add_pi16(a0, _mm_srli_si64(a0, 32));

		*dstPixelPtr = _m_to_int(a0);

		dstPixelPtr++;
		x += dx;
		y += dy;
	}

	_mm_empty();
}

/* ******************************************************************** */

#undef  MLIB_TYPE
#define	MLIB_TYPE	mlib_s16

/* ******************************************************************** */

void
mlib_m_ImageAffineTableLine_s16nw_4_4_2(
    __m64 * buff,
    __m64 * filterX,
    __m64 * filterY,
    mlib_u8 **lineAddr,
    mlib_affine_workspace * ws)
{
	DECLAREVAR __m64 yf0, yf1, yf2, yf3;
	__m64 a0, a1, a2, a3;
	__m64 b0, b1, b2, b3;

	dstPixelPtr = (mlib_s32 *)buff;

	for (i = 0; i < size; i++) {
		CALC_PTR yf0 = fptry[0];
		yf1 = fptry[1];
		yf2 = fptry[2];
		yf3 = fptry[3];

		a0 = _mm_mulhi_pi16(*(__m64 *) (sPtr), yf0);
		a1 = _mm_mulhi_pi16(*(__m64 *) (sPtr + srcStride), yf1);
		a2 = _mm_mulhi_pi16(*(__m64 *) (sPtr + 2 * srcStride), yf2);
		a3 = _mm_mulhi_pi16(*(__m64 *) (sPtr + 3 * srcStride), yf3);
		b0 = _mm_mulhi_pi16(*(__m64 *) (sPtr + 8), yf0);
		b1 = _mm_mulhi_pi16(*(__m64 *) (sPtr + srcStride + 8), yf1);
		b2 = _mm_mulhi_pi16(*(__m64 *) (sPtr + 2 * srcStride + 8), yf2);
		b3 = _mm_mulhi_pi16(*(__m64 *) (sPtr + 3 * srcStride + 8), yf3);
		a0 = _mm_add_pi16(a0, a1);
		a2 = _mm_add_pi16(a2, a3);
		a0 = _mm_add_pi16(a0, a2);
		b0 = _mm_add_pi16(b0, b1);
		b2 = _mm_add_pi16(b2, b3);
		b0 = _mm_add_pi16(b0, b2);
		a0 = _mm_mulhi_pi16(a0, fptrx[0]);
		b0 = _mm_mulhi_pi16(b0, fptrx[1]);
		a0 = _mm_add_pi16(a0, b0);
		a0 = _mm_add_pi16(a0, _mm_srli_si64(a0, 32));

		*dstPixelPtr = _m_to_int(a0);
		dstPixelPtr++;
		x += dx;
		y += dy;
	}

	_mm_empty();
}

/* ******************************************************************** */

void
mlib_m_ImageAffineTableLine_s16nw_3_4_2(
    __m64 * buff,
    __m64 * filterX,
    __m64 * filterY,
    mlib_u8 **lineAddr,
    mlib_affine_workspace * ws)
{
	DECLAREVAR __m64 yf0, yf1, yf2;
	__m64 a0, a1, a2;
	__m64 b0, b1, b2;

	dstPixelPtr = (mlib_s32 *)buff;

	for (i = 0; i < size; i++) {
		CALC_PTR yf0 = fptry[0];
		yf1 = fptry[1];
		yf2 = fptry[2];

		a0 = _mm_mulhi_pi16(*(__m64 *) (sPtr), yf0);
		a1 = _mm_mulhi_pi16(*(__m64 *) (sPtr + srcStride), yf1);
		a2 = _mm_mulhi_pi16(*(__m64 *) (sPtr + 2 * srcStride), yf2);
		b0 = _mm_mulhi_pi16(*(__m64 *) (sPtr + 8), yf0);
		b1 = _mm_mulhi_pi16(*(__m64 *) (sPtr + srcStride + 8), yf1);
		b2 = _mm_mulhi_pi16(*(__m64 *) (sPtr + 2 * srcStride + 8), yf2);
		a0 = _mm_add_pi16(a0, a1);
		a0 = _mm_add_pi16(a0, a2);
		b0 = _mm_add_pi16(b0, b1);
		b0 = _mm_add_pi16(b0, b2);
		a0 = _mm_mulhi_pi16(a0, fptrx[0]);
		b0 = _mm_mulhi_pi16(b0, fptrx[1]);
		a0 = _mm_add_pi16(a0, b0);
		a0 = _mm_add_pi16(a0, _mm_srli_si64(a0, 32));

		*dstPixelPtr = _m_to_int(a0);
		dstPixelPtr++;
		x += dx;
		y += dy;
	}

	_mm_empty();
}

/* ******************************************************************** */

void
mlib_m_ImageAffineTableLine_s16nw_2_4_2(
    __m64 * buff,
    __m64 * filterX,
    __m64 * filterY,
    mlib_u8 **lineAddr,
    mlib_affine_workspace * ws)
{
	DECLAREVAR __m64 a0, a1;
	__m64 yf0, yf1, b0, b1;

	dstPixelPtr = (mlib_s32 *)buff;

	for (i = 0; i < size; i++) {
		CALC_PTR yf0 = fptry[0];
		yf1 = fptry[1];

		a0 = _mm_mulhi_pi16(*(__m64 *) (sPtr), yf0);
		a1 = _mm_mulhi_pi16(*(__m64 *) (sPtr + srcStride), yf1);
		b0 = _mm_mulhi_pi16(*(__m64 *) (sPtr + 8), yf0);
		b1 = _mm_mulhi_pi16(*(__m64 *) (sPtr + srcStride + 8), yf1);
		a0 = _mm_add_pi16(a0, a1);
		b0 = _mm_add_pi16(b0, b1);
		a0 = _mm_mulhi_pi16(a0, fptrx[0]);
		b0 = _mm_mulhi_pi16(b0, fptrx[1]);
		a0 = _mm_add_pi16(a0, b0);
		a0 = _mm_add_pi16(a0, _mm_srli_si64(a0, 32));

		*dstPixelPtr = _m_to_int(a0);
		dstPixelPtr++;
		x += dx;
		y += dy;
	}

	_mm_empty();
}

/* ******************************************************************** */

void
mlib_m_ImageAffineTableLine_s16nw_4_2_2(
    __m64 * buff,
    __m64 * filterX,
    __m64 * filterY,
    mlib_u8 **lineAddr,
    mlib_affine_workspace * ws)
{
	DECLAREVAR __m64 yf0, yf1, yf2, yf3;
	__m64 a0, a1, a2, a3;

	dstPixelPtr = (mlib_s32 *)buff;

	for (i = 0; i < size; i++) {
		CALC_PTR yf0 = fptry[0];
		yf1 = fptry[1];
		yf2 = fptry[2];
		yf3 = fptry[3];

		a0 = _mm_mulhi_pi16(*(__m64 *) (sPtr), yf0);
		a1 = _mm_mulhi_pi16(*(__m64 *) (sPtr + srcStride), yf1);
		a2 = _mm_mulhi_pi16(*(__m64 *) (sPtr + 2 * srcStride), yf2);
		a3 = _mm_mulhi_pi16(*(__m64 *) (sPtr + 3 * srcStride), yf3);
		a0 = _mm_add_pi16(a0, a1);
		a2 = _mm_add_pi16(a2, a3);
		a0 = _mm_add_pi16(a0, a2);
		a0 = _mm_mulhi_pi16(a0, fptrx[0]);
		a0 = _mm_add_pi16(a0, _mm_srli_si64(a0, 32));

		*dstPixelPtr = _m_to_int(a0);
		dstPixelPtr++;
		x += dx;
		y += dy;
	}

	_mm_empty();
}

/* ******************************************************************** */

void
mlib_m_ImageAffineTableLine_s16nw_3_2_2(
    __m64 * buff,
    __m64 * filterX,
    __m64 * filterY,
    mlib_u8 **lineAddr,
    mlib_affine_workspace * ws)
{
	DECLAREVAR __m64 yf0, yf1, yf2;
	__m64 a0, a1, a2;

	dstPixelPtr = (mlib_s32 *)buff;

	for (i = 0; i < size; i++) {
		CALC_PTR yf0 = fptry[0];
		yf1 = fptry[1];
		yf2 = fptry[2];

		a0 = _mm_mulhi_pi16(*(__m64 *) (sPtr), yf0);
		a1 = _mm_mulhi_pi16(*(__m64 *) (sPtr + srcStride), yf1);
		a2 = _mm_mulhi_pi16(*(__m64 *) (sPtr + 2 * srcStride), yf2);
		a0 = _mm_add_pi16(a0, a1);
		a0 = _mm_add_pi16(a0, a2);
		a0 = _mm_mulhi_pi16(a0, fptrx[0]);
		a0 = _mm_add_pi16(a0, _mm_srli_si64(a0, 32));

		*dstPixelPtr = _m_to_int(a0);
		dstPixelPtr++;
		x += dx;
		y += dy;
	}

	_mm_empty();
}

/* ******************************************************************** */

void
mlib_m_ImageAffineTableLine_s16nw_2_2_2(
    __m64 * buff,
    __m64 * filterX,
    __m64 * filterY,
    mlib_u8 **lineAddr,
    mlib_affine_workspace * ws)
{
	DECLAREVAR __m64 yf0, yf1, a0, a1;

	dstPixelPtr = (mlib_s32 *)buff;

	for (i = 0; i < size; i++) {
		CALC_PTR yf0 = fptry[0];
		yf1 = fptry[1];

		a0 = _mm_mulhi_pi16(*(__m64 *) (sPtr), yf0);
		a1 = _mm_mulhi_pi16(*(__m64 *) (sPtr + srcStride), yf1);
		a0 = _mm_add_pi16(a0, a1);

		a0 = _mm_mulhi_pi16(a0, fptrx[0]);
		a0 = _mm_add_pi16(a0, _mm_srli_si64(a0, 32));

		*dstPixelPtr = _m_to_int(a0);

		dstPixelPtr++;
		x += dx;
		y += dy;
	}

	_mm_empty();
}

/* ******************************************************************** */

void
mlib_m_ImageAffineTableLine_u16nw_4_4_2(
    __m64 * buff,
    __m64 * filterX,
    __m64 * filterY,
    mlib_u8 **lineAddr,
    mlib_affine_workspace * ws)
{
	DECLAREVAR __m64 a0, a1, a2, a3;
	__m64 b0, b1, b2, b3;

	dstPixelPtr = (mlib_s32 *)buff;

	for (i = 0; i < size; i++) {
		CALC_PTR a0 = _mm_srli_pi16(*(__m64 *) (sPtr), 1);
		a1 = _mm_srli_pi16(*(__m64 *) (sPtr + srcStride), 1);
		a2 = _mm_srli_pi16(*(__m64 *) (sPtr + 2 * srcStride), 1);
		a3 = _mm_srli_pi16(*(__m64 *) (sPtr + 3 * srcStride), 1);
		b0 = _mm_srli_pi16(*(__m64 *) (sPtr + 8), 1);
		b1 = _mm_srli_pi16(*(__m64 *) (sPtr + srcStride + 8), 1);
		b2 = _mm_srli_pi16(*(__m64 *) (sPtr + 2 * srcStride + 8), 1);
		b3 = _mm_srli_pi16(*(__m64 *) (sPtr + 3 * srcStride + 8), 1);
		a0 = _mm_mulhi_pi16(a0, fptry[0]);
		a1 = _mm_mulhi_pi16(a1, fptry[1]);
		a2 = _mm_mulhi_pi16(a2, fptry[2]);
		a3 = _mm_mulhi_pi16(a3, fptry[3]);
		b0 = _mm_mulhi_pi16(b0, fptry[0]);
		b1 = _mm_mulhi_pi16(b1, fptry[1]);
		b2 = _mm_mulhi_pi16(b2, fptry[2]);
		b3 = _mm_mulhi_pi16(b3, fptry[3]);
		a0 = _mm_add_pi16(a0, a1);
		a2 = _mm_add_pi16(a2, a3);
		a0 = _mm_add_pi16(a0, a2);
		b0 = _mm_add_pi16(b0, b1);
		b2 = _mm_add_pi16(b2, b3);
		b0 = _mm_add_pi16(b0, b2);
		a0 = _mm_mulhi_pi16(a0, fptrx[0]);
		b0 = _mm_mulhi_pi16(b0, fptrx[1]);
		a0 = _mm_add_pi16(a0, b0);
		a0 = _mm_add_pi16(a0, _mm_srli_si64(a0, 32));

		*dstPixelPtr = _m_to_int(a0);
		dstPixelPtr++;
		x += dx;
		y += dy;
	}

	_mm_empty();
}

/* ******************************************************************** */

void
mlib_m_ImageAffineTableLine_u16nw_3_4_2(
    __m64 * buff,
    __m64 * filterX,
    __m64 * filterY,
    mlib_u8 **lineAddr,
    mlib_affine_workspace * ws)
{
	DECLAREVAR __m64 a0, a1, a2;
	__m64 b0, b1, b2;

	dstPixelPtr = (mlib_s32 *)buff;

	for (i = 0; i < size; i++) {
		CALC_PTR a0 = _mm_srli_pi16(*(__m64 *) (sPtr), 1);
		a1 = _mm_srli_pi16(*(__m64 *) (sPtr + srcStride), 1);
		a2 = _mm_srli_pi16(*(__m64 *) (sPtr + 2 * srcStride), 1);
		b0 = _mm_srli_pi16(*(__m64 *) (sPtr + 8), 1);
		b1 = _mm_srli_pi16(*(__m64 *) (sPtr + srcStride + 8), 1);
		b2 = _mm_srli_pi16(*(__m64 *) (sPtr + 2 * srcStride + 8), 1);
		a0 = _mm_mulhi_pi16(a0, fptry[0]);
		a1 = _mm_mulhi_pi16(a1, fptry[1]);
		a2 = _mm_mulhi_pi16(a2, fptry[2]);
		b0 = _mm_mulhi_pi16(b0, fptry[0]);
		b1 = _mm_mulhi_pi16(b1, fptry[1]);
		b2 = _mm_mulhi_pi16(b2, fptry[2]);
		a0 = _mm_add_pi16(a0, a1);
		a0 = _mm_add_pi16(a0, a2);
		b0 = _mm_add_pi16(b0, b1);
		b0 = _mm_add_pi16(b0, b2);
		a0 = _mm_mulhi_pi16(a0, fptrx[0]);
		b0 = _mm_mulhi_pi16(b0, fptrx[1]);
		a0 = _mm_add_pi16(a0, b0);
		a0 = _mm_add_pi16(a0, _mm_srli_si64(a0, 32));

		*dstPixelPtr = _m_to_int(a0);
		dstPixelPtr++;
		x += dx;
		y += dy;
	}

	_mm_empty();
}

/* ******************************************************************** */

void
mlib_m_ImageAffineTableLine_u16nw_2_4_2(
    __m64 * buff,
    __m64 * filterX,
    __m64 * filterY,
    mlib_u8 **lineAddr,
    mlib_affine_workspace * ws)
{
	DECLAREVAR __m64 a0, a1;
	__m64 b0, b1;

	dstPixelPtr = (mlib_s32 *)buff;

	for (i = 0; i < size; i++) {
		CALC_PTR a0 = _mm_srli_pi16(*(__m64 *) (sPtr), 1);
		a1 = _mm_srli_pi16(*(__m64 *) (sPtr + srcStride), 1);
		b0 = _mm_srli_pi16(*(__m64 *) (sPtr + 8), 1);
		b1 = _mm_srli_pi16(*(__m64 *) (sPtr + srcStride + 8), 1);
		a0 = _mm_mulhi_pi16(a0, fptry[0]);
		a1 = _mm_mulhi_pi16(a1, fptry[1]);
		b0 = _mm_mulhi_pi16(b0, fptry[0]);
		b1 = _mm_mulhi_pi16(b1, fptry[1]);
		a0 = _mm_add_pi16(a0, a1);
		b0 = _mm_add_pi16(b0, b1);
		a0 = _mm_mulhi_pi16(a0, fptrx[0]);
		b0 = _mm_mulhi_pi16(b0, fptrx[1]);
		a0 = _mm_add_pi16(a0, b0);
		a0 = _mm_add_pi16(a0, _mm_srli_si64(a0, 32));

		*dstPixelPtr = _m_to_int(a0);
		dstPixelPtr++;
		x += dx;
		y += dy;
	}

	_mm_empty();
}

/* ******************************************************************** */

void
mlib_m_ImageAffineTableLine_u16nw_4_2_2(
    __m64 * buff,
    __m64 * filterX,
    __m64 * filterY,
    mlib_u8 **lineAddr,
    mlib_affine_workspace * ws)
{
	DECLAREVAR __m64 a0, a1, a2, a3;

	dstPixelPtr = (mlib_s32 *)buff;

	for (i = 0; i < size; i++) {
		CALC_PTR a0 = _mm_srli_pi16(*(__m64 *) (sPtr), 1);
		a1 = _mm_srli_pi16(*(__m64 *) (sPtr + srcStride), 1);
		a2 = _mm_srli_pi16(*(__m64 *) (sPtr + 2 * srcStride), 1);
		a3 = _mm_srli_pi16(*(__m64 *) (sPtr + 3 * srcStride), 1);
		a0 = _mm_mulhi_pi16(a0, fptry[0]);
		a1 = _mm_mulhi_pi16(a1, fptry[1]);
		a2 = _mm_mulhi_pi16(a2, fptry[2]);
		a3 = _mm_mulhi_pi16(a3, fptry[3]);
		a0 = _mm_add_pi16(a0, a1);
		a2 = _mm_add_pi16(a2, a3);
		a0 = _mm_add_pi16(a0, a2);
		a0 = _mm_mulhi_pi16(a0, fptrx[0]);
		a0 = _mm_add_pi16(a0, _mm_srli_si64(a0, 32));

		*dstPixelPtr = _m_to_int(a0);
		dstPixelPtr++;
		x += dx;
		y += dy;
	}

	_mm_empty();
}

/* ******************************************************************** */

void
mlib_m_ImageAffineTableLine_u16nw_3_2_2(
    __m64 * buff,
    __m64 * filterX,
    __m64 * filterY,
    mlib_u8 **lineAddr,
    mlib_affine_workspace * ws)
{
	DECLAREVAR __m64 a0, a1, a2;

	dstPixelPtr = (mlib_s32 *)buff;

	for (i = 0; i < size; i++) {
		CALC_PTR a0 = _mm_srli_pi16(*(__m64 *) (sPtr), 1);
		a1 = _mm_srli_pi16(*(__m64 *) (sPtr + srcStride), 1);
		a2 = _mm_srli_pi16(*(__m64 *) (sPtr + 2 * srcStride), 1);
		a0 = _mm_mulhi_pi16(a0, fptry[0]);
		a1 = _mm_mulhi_pi16(a1, fptry[1]);
		a2 = _mm_mulhi_pi16(a2, fptry[2]);
		a0 = _mm_add_pi16(a0, a1);
		a0 = _mm_add_pi16(a0, a2);
		a0 = _mm_mulhi_pi16(a0, fptrx[0]);
		a0 = _mm_add_pi16(a0, _mm_srli_si64(a0, 32));

		*dstPixelPtr = _m_to_int(a0);
		dstPixelPtr++;
		x += dx;
		y += dy;
	}

	_mm_empty();
}

/* ******************************************************************** */

void
mlib_m_ImageAffineTableLine_u16nw_2_2_2(
    __m64 * buff,
    __m64 * filterX,
    __m64 * filterY,
    mlib_u8 **lineAddr,
    mlib_affine_workspace * ws)
{
	DECLAREVAR __m64 a0, a1;

	dstPixelPtr = (mlib_s32 *)buff;

	for (i = 0; i < size; i++) {
		CALC_PTR a0 = _mm_srli_pi16(*(__m64 *) (sPtr), 1);
		a1 = _mm_srli_pi16(*(__m64 *) (sPtr + srcStride), 1);
		a0 = _mm_mulhi_pi16(a0, fptry[0]);
		a1 = _mm_mulhi_pi16(a1, fptry[1]);
		a0 = _mm_add_pi16(a0, a1);

		a0 = _mm_mulhi_pi16(a0, fptrx[0]);
		a0 = _mm_add_pi16(a0, _mm_srli_si64(a0, 32));

		*dstPixelPtr = _m_to_int(a0);

		dstPixelPtr++;
		x += dx;
		y += dy;
	}

	_mm_empty();
}

/* ******************************************************************** */
