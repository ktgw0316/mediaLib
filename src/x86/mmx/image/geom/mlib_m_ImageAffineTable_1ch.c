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

#pragma ident	"@(#)mlib_m_ImageAffineTable_1ch.c	9.4	07/11/05 SMI"

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
	mlib_u8 *sPtr0;                                         \
	mlib_s16 *dstPixelPtr;                                  \
	mlib_s32 srcStride = ws->srcStride;                     \
	mlib_s32 i, filterpos, xSrc, ySrc;                      \
	mlib_s32 dx = ws->dx;                                   \
	mlib_s32 dy = ws->dy;                                   \
	mlib_s32 x_shift = ws->x_shift0;                        \
	mlib_s32 y_shift = ws->y_shift;                         \
	mlib_s32 xf_shift = ws->xf_shift;                       \
	mlib_s32 xf_mask = ws->xf_mask;                         \
	mlib_s32 yf_shift = ws->yf_shift;                       \
	mlib_s32 yf_mask = ws->yf_mask;                         \
	mlib_s32 size = ws->size;                               \
	mlib_s32 x = ws->x0;                                    \
	mlib_s32 y = ws->y0;                                    \
	mlib_s32 ri;                                            \
	__m64 *fptr0;                                           \
	__m64 xf0, r0, r1, d0;
/* ******************************************************************** */
#define	CALC_PTR(i)                                                 \
	filterpos = (x >> xf_shift) & xf_mask;                      \
	xf##i = *((__m64 *) ((mlib_u8 *)filterX + filterpos));      \
	filterpos = (y >> yf_shift) & yf_mask;                      \
	fptr##i = (__m64 *) ((mlib_u8 *)filterY + 4 * filterpos);   \
	xSrc = (x >> x_shift);                                      \
	ySrc = (y >> y_shift);                                      \
	sPtr##i =                                                   \
	    (mlib_u8 *)(((MLIB_TYPE **) lineAddr)[ySrc] + xSrc);
/* ******************************************************************** */

#define	MLIB_TYPE	mlib_u8

/* ******************************************************************** */

void
mlib_m_ImageAffineTableLine_8nw_4_4_1(
    __m64 * buff,
    __m64 * filterX,
    __m64 * filterY,
    mlib_u8 **lineAddr,
    mlib_affine_workspace * ws)
{
	DECLAREVAR __m64 _m_mzero[1] = {
	0};
	__m64 yf0, yf1, yf2, yf3, r2, r3;

	dstPixelPtr = (mlib_s16 *)buff;

	for (i = 0; i < size; i++) {
		CALC_PTR(0)
		    yf0 = fptr0[0];
		yf1 = fptr0[1];
		yf2 = fptr0[2];
		yf3 = fptr0[3];
		r0 = mmx_fmul8x16hi(*(__m64 *) (sPtr0), yf0);
		r1 = mmx_fmul8x16hi(*(__m64 *) (sPtr0 + srcStride), yf1);
		r2 = mmx_fmul8x16hi(*(__m64 *) (sPtr0 + 2 * srcStride), yf2);
		r3 = mmx_fmul8x16hi(*(__m64 *) (sPtr0 + 3 * srcStride), yf3);
		r0 = _mm_add_pi16(r0, r1);
		r2 = _mm_add_pi16(r2, r3);
		d0 = _mm_add_pi16(r0, r2);
		d0 = _mm_madd_pi16(d0, xf0);
		d0 = _mm_add_pi32(d0, _mm_srli_si64(d0, 32));
		d0 = _mm_srai_pi32(d0, 16);
		ri = _m_to_int(_mm_packs_pi32(d0, _m_mzero[0]));
		*dstPixelPtr = ri;
		dstPixelPtr++;
		x += dx;
		y += dy;
	}

	_mm_empty();
}

/* ******************************************************************** */

void
mlib_m_ImageAffineTableLine_8nw_3_4_1(
    __m64 * buff,
    __m64 * filterX,
    __m64 * filterY,
    mlib_u8 **lineAddr,
    mlib_affine_workspace * ws)
{
	DECLAREVAR __m64 _m_mzero[1] = {
	0};
	__m64 yf0, yf1, yf2, r2;

	dstPixelPtr = (mlib_s16 *)buff;

	for (i = 0; i < size; i++) {
		CALC_PTR(0)
		    yf0 = fptr0[0];
		yf1 = fptr0[1];
		yf2 = fptr0[2];
		r0 = mmx_fmul8x16hi(*(__m64 *) (sPtr0), yf0);
		r1 = mmx_fmul8x16hi(*(__m64 *) (sPtr0 + srcStride), yf1);
		r2 = mmx_fmul8x16hi(*(__m64 *) (sPtr0 + 2 * srcStride), yf2);
		r0 = _mm_add_pi16(r0, r1);
		d0 = _mm_add_pi16(r0, r2);
		d0 = _mm_madd_pi16(d0, xf0);
		d0 = _mm_add_pi32(d0, _mm_srli_si64(d0, 32));
		d0 = _mm_srai_pi32(d0, 16);
		ri = _m_to_int(_mm_packs_pi32(d0, _m_mzero[0]));
		*dstPixelPtr = ri;
		dstPixelPtr++;
		x += dx;
		y += dy;
	}

	_mm_empty();
}

/* ******************************************************************** */

void
mlib_m_ImageAffineTableLine_8nw_2_4_1(
    __m64 * buff,
    __m64 * filterX,
    __m64 * filterY,
    mlib_u8 **lineAddr,
    mlib_affine_workspace * ws)
{
	DECLAREVAR __m64 _m_mzero[1] = {
	0};
	__m64 yf0, yf1;

	dstPixelPtr = (mlib_s16 *)buff;

	for (i = 0; i < size; i++) {
		CALC_PTR(0)
		    yf0 = fptr0[0];
		yf1 = fptr0[1];
		r0 = mmx_fmul8x16hi(*(__m64 *) (sPtr0), yf0);
		r1 = mmx_fmul8x16hi(*(__m64 *) (sPtr0 + srcStride), yf1);
		d0 = _mm_add_pi16(r0, r1);
		d0 = _mm_madd_pi16(d0, xf0);
		d0 = _mm_add_pi32(d0, _mm_srli_si64(d0, 32));
		d0 = _mm_srai_pi32(d0, 16);
		ri = _m_to_int(_mm_packs_pi32(d0, _m_mzero[0]));
		*dstPixelPtr = ri;
		dstPixelPtr++;
		x += dx;
		y += dy;
	}

	_mm_empty();
}

/* ******************************************************************** */

void
mlib_m_ImageAffineTableLine_8nw_4_2_1(
    __m64 * buff,
    __m64 * filterX,
    __m64 * filterY,
    mlib_u8 **lineAddr,
    mlib_affine_workspace * ws)
{
	DECLAREVAR mlib_u8 *sPtr1;
	__m64 *fptr1, xf1;
	mlib_s32 s0, s1, s2, s3;
	__m64 _m_mzero[1] = { 0 };
	__m64 yf0, yf1, yf2, yf3, r2, r3;

	dstPixelPtr = (mlib_s16 *)buff;

	for (i = 0; i < size - 1; i += 2) {
		CALC_PTR(0)
		    x += dx;
		y += dy;
		CALC_PTR(1)
		    s0 = ((*(mlib_s32 *)(sPtr1)) << 16) |
		    ((*(mlib_s32 *)(sPtr0)) & 0xFFFF);
		s1 = ((*(mlib_s32 *)(sPtr1 + srcStride)) << 16) |
		    ((*(mlib_s32 *)(sPtr0 + srcStride)) & 0xFFFF);
		s2 = ((*(mlib_s32 *)(sPtr1 + 2 * srcStride)) << 16) |
		    ((*(mlib_s32 *)(sPtr0 + 2 * srcStride)) & 0xFFFF);
		s3 = ((*(mlib_s32 *)(sPtr1 + 3 * srcStride)) << 16) |
		    ((*(mlib_s32 *)(sPtr0 + 3 * srcStride)) & 0xFFFF);

		yf0 = _mm_unpackhi_pi32(fptr0[0], fptr1[0]);
		yf1 = _mm_unpackhi_pi32(fptr0[1], fptr1[1]);
		yf2 = _mm_unpackhi_pi32(fptr0[2], fptr1[2]);
		yf3 = _mm_unpackhi_pi32(fptr0[3], fptr1[3]);

		xf0 = _mm_unpacklo_pi32(xf0, xf1);

		r0 = mmx_fmul8x16hi(_m_from_int(s0), yf0);
		r1 = mmx_fmul8x16hi(_m_from_int(s1), yf1);
		r2 = mmx_fmul8x16hi(_m_from_int(s2), yf2);
		r3 = mmx_fmul8x16hi(_m_from_int(s3), yf3);
		r0 = _mm_add_pi16(r0, r1);
		r2 = _mm_add_pi16(r2, r3);
		d0 = _mm_add_pi16(r0, r2);
		d0 = _mm_madd_pi16(d0, xf0);
		d0 = _mm_srai_pi32(d0, 16);
		ri = _m_to_int(_mm_packs_pi32(d0, _m_mzero[0]));
		*(mlib_s32 *)dstPixelPtr = ri;

		dstPixelPtr += 2;
		x += dx;
		y += dy;
	}

	if (i < size) {
		CALC_PTR(0)

		    s0 = *(mlib_s32 *)(sPtr0);
		s1 = *(mlib_s32 *)(sPtr0 + srcStride);
		s2 = *(mlib_s32 *)(sPtr0 + 2 * srcStride);
		s3 = *(mlib_s32 *)(sPtr0 + 3 * srcStride);

		yf0 = fptr0[0];
		yf1 = fptr0[1];
		yf2 = fptr0[2];
		yf3 = fptr0[3];

		r0 = mmx_fmul8x16hi(_m_from_int(s0), yf0);
		r1 = mmx_fmul8x16hi(_m_from_int(s1), yf1);
		r2 = mmx_fmul8x16hi(_m_from_int(s2), yf2);
		r3 = mmx_fmul8x16hi(_m_from_int(s3), yf3);
		r0 = _mm_add_pi16(r0, r1);
		r2 = _mm_add_pi16(r2, r3);
		d0 = _mm_add_pi16(r0, r2);
		d0 = _mm_madd_pi16(d0, xf0);
		d0 = _mm_srai_pi32(d0, 16);
		ri = _m_to_int(_mm_packs_pi32(d0, _m_mzero[0]));
		*(mlib_s32 *)dstPixelPtr = ri;
	}

	_mm_empty();
}

/* ******************************************************************** */

void
mlib_m_ImageAffineTableLine_8nw_3_2_1(
    __m64 * buff,
    __m64 * filterX,
    __m64 * filterY,
    mlib_u8 **lineAddr,
    mlib_affine_workspace * ws)
{
	DECLAREVAR mlib_u8 *sPtr1;
	__m64 *fptr1, xf1;
	mlib_s32 s0, s1, s2;
	__m64 _m_mzero[1] = { 0 };
	__m64 yf0, yf1, yf2, r2;

	dstPixelPtr = (mlib_s16 *)buff;

	for (i = 0; i < size - 1; i += 2) {
		CALC_PTR(0)
		    x += dx;
		y += dy;
		CALC_PTR(1)
		    s0 = ((*(mlib_s32 *)(sPtr1)) << 16) |
		    ((*(mlib_s32 *)(sPtr0)) & 0xFFFF);
		s1 = ((*(mlib_s32 *)(sPtr1 + srcStride)) << 16) |
		    ((*(mlib_s32 *)(sPtr0 + srcStride)) & 0xFFFF);
		s2 = ((*(mlib_s32 *)(sPtr1 + 2 * srcStride)) << 16) |
		    ((*(mlib_s32 *)(sPtr0 + 2 * srcStride)) & 0xFFFF);

		yf0 = _mm_unpackhi_pi32(fptr0[0], fptr1[0]);
		yf1 = _mm_unpackhi_pi32(fptr0[1], fptr1[1]);
		yf2 = _mm_unpackhi_pi32(fptr0[2], fptr1[2]);

		xf0 = _mm_unpacklo_pi32(xf0, xf1);

		r0 = mmx_fmul8x16hi(_m_from_int(s0), yf0);
		r1 = mmx_fmul8x16hi(_m_from_int(s1), yf1);
		r2 = mmx_fmul8x16hi(_m_from_int(s2), yf2);
		r0 = _mm_add_pi16(r0, r1);
		d0 = _mm_add_pi16(r0, r2);
		d0 = _mm_madd_pi16(d0, xf0);
		d0 = _mm_srai_pi32(d0, 16);
		ri = _m_to_int(_mm_packs_pi32(d0, _m_mzero[0]));
		*(mlib_s32 *)dstPixelPtr = ri;

		dstPixelPtr += 2;
		x += dx;
		y += dy;
	}

	if (i < size) {
		CALC_PTR(0)

		    s0 = *(mlib_s32 *)(sPtr0);
		s1 = *(mlib_s32 *)(sPtr0 + srcStride);
		s2 = *(mlib_s32 *)(sPtr0 + 2 * srcStride);

		yf0 = fptr0[0];
		yf1 = fptr0[1];
		yf2 = fptr0[2];

		r0 = mmx_fmul8x16hi(_m_from_int(s0), yf0);
		r1 = mmx_fmul8x16hi(_m_from_int(s1), yf1);
		r2 = mmx_fmul8x16hi(_m_from_int(s2), yf2);
		r0 = _mm_add_pi16(r0, r1);
		d0 = _mm_add_pi16(r0, r2);
		d0 = _mm_madd_pi16(d0, xf0);
		d0 = _mm_srai_pi32(d0, 16);
		ri = _m_to_int(_mm_packs_pi32(d0, _m_mzero[0]));
		*(mlib_s32 *)dstPixelPtr = ri;
	}
	_mm_empty();
}

/* ******************************************************************** */

void
mlib_m_ImageAffineTableLine_8nw_2_2_1(
    __m64 * buff,
    __m64 * filterX,
    __m64 * filterY,
    mlib_u8 **lineAddr,
    mlib_affine_workspace * ws)
{
	DECLAREVAR mlib_u8 *sPtr1;
	__m64 *fptr1, xf1, yf0, yf1;
	mlib_s32 s0, s1;
	__m64 _m_mzero[1] = { 0 };

	dstPixelPtr = (mlib_s16 *)buff;

	for (i = 0; i < size - 1; i += 2) {
		CALC_PTR(0)
		    x += dx;
		y += dy;
		CALC_PTR(1)
		    s0 = ((*(mlib_s32 *)(sPtr1)) << 16) |
		    ((*(mlib_s32 *)(sPtr0)) & 0xFFFF);
		s1 = ((*(mlib_s32 *)(sPtr1 + srcStride)) << 16) |
		    ((*(mlib_s32 *)(sPtr0 + srcStride)) & 0xFFFF);

		yf0 = _mm_unpackhi_pi32(fptr0[0], fptr1[0]);
		yf1 = _mm_unpackhi_pi32(fptr0[1], fptr1[1]);

		xf0 = _mm_unpacklo_pi32(xf0, xf1);

		r0 = mmx_fmul8x16hi(_m_from_int(s0), yf0);
		r1 = mmx_fmul8x16hi(_m_from_int(s1), yf1);
		d0 = _mm_add_pi16(r0, r1);
		d0 = _mm_madd_pi16(d0, xf0);
		d0 = _mm_srai_pi32(d0, 16);
		ri = _m_to_int(_mm_packs_pi32(d0, _m_mzero[0]));

		*(mlib_s32 *)dstPixelPtr = ri;

		dstPixelPtr += 2;
		x += dx;
		y += dy;
	}

	if (i < size) {
		CALC_PTR(0)

		    s0 = *(mlib_s32 *)(sPtr0);
		s1 = *(mlib_s32 *)(sPtr0 + srcStride);

		yf0 = fptr0[0];
		yf1 = fptr0[1];

		r0 = mmx_fmul8x16hi(_m_from_int(s0), yf0);
		r1 = mmx_fmul8x16hi(_m_from_int(s1), yf1);
		d0 = _mm_add_pi16(r0, r1);
		d0 = _mm_madd_pi16(d0, xf0);
		d0 = _mm_srai_pi32(d0, 16);
		ri = _m_to_int(_mm_packs_pi32(d0, _m_mzero[0]));
		*(mlib_s32 *)dstPixelPtr = ri;
	}
	_mm_empty();
}

/* ******************************************************************** */

#undef  MLIB_TYPE
#define	MLIB_TYPE	mlib_s16

/* ******************************************************************** */

void
mlib_m_ImageAffineTableLine_s16nw_4_4_1(
    __m64 * buff,
    __m64 * filterX,
    __m64 * filterY,
    mlib_u8 **lineAddr,
    mlib_affine_workspace * ws)
{
	DECLAREVAR __m64 _m_mzero[1] = {
	0};
	__m64 yf0, yf1, yf2, yf3, r2, r3;

	dstPixelPtr = (mlib_s16 *)buff;

	for (i = 0; i < size; i++) {
		CALC_PTR(0)
		    yf0 = fptr0[0];
		yf1 = fptr0[1];
		yf2 = fptr0[2];
		yf3 = fptr0[3];
		r0 = _mm_mulhi_pi16(*(__m64 *) (sPtr0), yf0);
		r1 = _mm_mulhi_pi16(*(__m64 *) (sPtr0 + srcStride), yf1);
		r2 = _mm_mulhi_pi16(*(__m64 *) (sPtr0 + 2 * srcStride), yf2);
		r3 = _mm_mulhi_pi16(*(__m64 *) (sPtr0 + 3 * srcStride), yf3);
		r0 = _mm_add_pi16(r0, r1);
		r2 = _mm_add_pi16(r2, r3);
		d0 = _mm_add_pi16(r0, r2);
		d0 = _mm_madd_pi16(d0, xf0);
		d0 = _mm_add_pi32(d0, _mm_srli_si64(d0, 32));
		d0 = _mm_srai_pi32(d0, 16);
		ri = _m_to_int(_mm_packs_pi32(d0, _m_mzero[0]));
		*dstPixelPtr = ri;
		dstPixelPtr++;
		x += dx;
		y += dy;
	}

	_mm_empty();
}

/* ******************************************************************** */

void
mlib_m_ImageAffineTableLine_s16nw_3_4_1(
    __m64 * buff,
    __m64 * filterX,
    __m64 * filterY,
    mlib_u8 **lineAddr,
    mlib_affine_workspace * ws)
{
	DECLAREVAR __m64 _m_mzero[1] = {
	0};
	__m64 yf0, yf1, yf2, r2;

	dstPixelPtr = (mlib_s16 *)buff;

	for (i = 0; i < size; i++) {
		CALC_PTR(0)
		    yf0 = fptr0[0];
		yf1 = fptr0[1];
		yf2 = fptr0[2];
		r0 = _mm_mulhi_pi16(*(__m64 *) (sPtr0), yf0);
		r1 = _mm_mulhi_pi16(*(__m64 *) (sPtr0 + srcStride), yf1);
		r2 = _mm_mulhi_pi16(*(__m64 *) (sPtr0 + 2 * srcStride), yf2);
		r0 = _mm_add_pi16(r0, r1);
		d0 = _mm_add_pi16(r0, r2);
		d0 = _mm_madd_pi16(d0, xf0);
		d0 = _mm_add_pi32(d0, _mm_srli_si64(d0, 32));
		d0 = _mm_srai_pi32(d0, 16);
		ri = _m_to_int(_mm_packs_pi32(d0, _m_mzero[0]));
		*dstPixelPtr = ri;
		dstPixelPtr++;
		x += dx;
		y += dy;
	}

	_mm_empty();
}

/* ******************************************************************** */

void
mlib_m_ImageAffineTableLine_s16nw_2_4_1(
    __m64 * buff,
    __m64 * filterX,
    __m64 * filterY,
    mlib_u8 **lineAddr,
    mlib_affine_workspace * ws)
{
	DECLAREVAR __m64 _m_mzero[1] = {
	0};
	__m64 yf0, yf1;

	dstPixelPtr = (mlib_s16 *)buff;

	for (i = 0; i < size; i++) {
		CALC_PTR(0)
		    yf0 = fptr0[0];
		yf1 = fptr0[1];
		r0 = _mm_mulhi_pi16(*(__m64 *) (sPtr0), yf0);
		r1 = _mm_mulhi_pi16(*(__m64 *) (sPtr0 + srcStride), yf1);
		d0 = _mm_add_pi16(r0, r1);
		d0 = _mm_madd_pi16(d0, xf0);
		d0 = _mm_add_pi32(d0, _mm_srli_si64(d0, 32));
		d0 = _mm_srai_pi32(d0, 16);
		ri = _m_to_int(_mm_packs_pi32(d0, _m_mzero[0]));
		*dstPixelPtr = ri;
		dstPixelPtr++;
		x += dx;
		y += dy;
	}

	_mm_empty();
}

/* ******************************************************************** */

void
mlib_m_ImageAffineTableLine_s16nw_4_2_1(
    __m64 * buff,
    __m64 * filterX,
    __m64 * filterY,
    mlib_u8 **lineAddr,
    mlib_affine_workspace * ws)
{
	DECLAREVAR mlib_u8 *sPtr1;
	__m64 *fptr1, xf1;
	__m64 s0, s1, s2, s3;
	__m64 yf0, yf1, yf2, yf3, r2, r3;
	__m64 _m_mzero[1] = { 0 };
	__m64 mask = _m_from_int(0xFFFFFFFF);

	dstPixelPtr = (mlib_s16 *)buff;

	for (i = 0; i < size - 1; i += 2) {
		CALC_PTR(0)
		    x += dx;
		y += dy;
		CALC_PTR(1)

		    s0 = _mm_or_si64(_mm_slli_si64((*(__m64 *) (sPtr1)), 32),
		    _mm_and_si64((*(__m64 *) (sPtr0)), mask));

		s1 = _mm_or_si64(_mm_slli_si64((*(__m64 *) (sPtr1 + srcStride)),
		    32), _mm_and_si64((*(__m64 *) (sPtr0 + srcStride)), mask));

		s2 = _mm_or_si64(_mm_slli_si64((*(__m64 *) (sPtr1 +
		    2 * srcStride)), 32),
		    _mm_and_si64((*(__m64 *) (sPtr0 + 2 * srcStride)), mask));

		s3 = _mm_or_si64(_mm_slli_si64((*(__m64 *) (sPtr1 +
		    3 * srcStride)), 32),
		    _mm_and_si64((*(__m64 *) (sPtr0 + 3 * srcStride)), mask));

		yf0 = _mm_unpackhi_pi32(fptr0[0], fptr1[0]);
		yf1 = _mm_unpackhi_pi32(fptr0[1], fptr1[1]);
		yf2 = _mm_unpackhi_pi32(fptr0[2], fptr1[2]);
		yf3 = _mm_unpackhi_pi32(fptr0[3], fptr1[3]);

		xf0 = _mm_unpacklo_pi32(xf0, xf1);

		r0 = _mm_mulhi_pi16(s0, yf0);
		r1 = _mm_mulhi_pi16(s1, yf1);
		r2 = _mm_mulhi_pi16(s2, yf2);
		r3 = _mm_mulhi_pi16(s3, yf3);
		r0 = _mm_add_pi16(r0, r1);
		r2 = _mm_add_pi16(r2, r3);
		d0 = _mm_add_pi16(r0, r2);
		d0 = _mm_madd_pi16(d0, xf0);
		d0 = _mm_srai_pi32(d0, 16);
		ri = _m_to_int(_mm_packs_pi32(d0, _m_mzero[0]));

		*(mlib_s32 *)dstPixelPtr = ri;

		dstPixelPtr += 2;
		x += dx;
		y += dy;
	}

	if (i < size) {
		CALC_PTR(0)

		    s0 = *(__m64 *) (sPtr0);
		s1 = *(__m64 *) (sPtr0 + srcStride);
		s2 = *(__m64 *) (sPtr0 + 2 * srcStride);
		s3 = *(__m64 *) (sPtr0 + 3 * srcStride);

		yf0 = fptr0[0];
		yf1 = fptr0[1];
		yf2 = fptr0[2];
		yf3 = fptr0[3];

		r0 = _mm_mulhi_pi16(s0, yf0);
		r1 = _mm_mulhi_pi16(s1, yf1);
		r2 = _mm_mulhi_pi16(s2, yf2);
		r3 = _mm_mulhi_pi16(s3, yf3);
		r0 = _mm_add_pi16(r0, r1);
		r2 = _mm_add_pi16(r2, r3);
		d0 = _mm_add_pi16(r0, r2);
		d0 = _mm_madd_pi16(d0, xf0);
		d0 = _mm_srai_pi32(d0, 16);
		ri = _m_to_int(_mm_packs_pi32(d0, _m_mzero[0]));

		*(mlib_s32 *)dstPixelPtr = ri;
	}
	_mm_empty();
}

/* ******************************************************************** */

void
mlib_m_ImageAffineTableLine_s16nw_3_2_1(
    __m64 * buff,
    __m64 * filterX,
    __m64 * filterY,
    mlib_u8 **lineAddr,
    mlib_affine_workspace * ws)
{
	DECLAREVAR mlib_u8 *sPtr1;
	__m64 *fptr1, xf1;
	__m64 s0, s1, s2;
	__m64 yf0, yf1, yf2, r2;
	__m64 _m_mzero[1] = { 0 };
	__m64 mask = _m_from_int(0xFFFFFFFF);

	dstPixelPtr = (mlib_s16 *)buff;

	for (i = 0; i < size - 1; i += 2) {
		CALC_PTR(0)
		    x += dx;
		y += dy;
		CALC_PTR(1)

		    s0 = _mm_or_si64(_mm_slli_si64((*(__m64 *) (sPtr1)), 32),
		    _mm_and_si64((*(__m64 *) (sPtr0)), mask));

		s1 = _mm_or_si64(_mm_slli_si64((*(__m64 *) (sPtr1 + srcStride)),
		    32), _mm_and_si64((*(__m64 *) (sPtr0 + srcStride)), mask));

		s2 = _mm_or_si64(_mm_slli_si64((*(__m64 *) (sPtr1 +
		    2 * srcStride)), 32),
		    _mm_and_si64((*(__m64 *) (sPtr0 + 2 * srcStride)), mask));

		yf0 = _mm_unpackhi_pi32(fptr0[0], fptr1[0]);
		yf1 = _mm_unpackhi_pi32(fptr0[1], fptr1[1]);
		yf2 = _mm_unpackhi_pi32(fptr0[2], fptr1[2]);

		xf0 = _mm_unpacklo_pi32(xf0, xf1);

		r0 = _mm_mulhi_pi16(s0, yf0);
		r1 = _mm_mulhi_pi16(s1, yf1);
		r2 = _mm_mulhi_pi16(s2, yf2);
		r0 = _mm_add_pi16(r0, r1);
		d0 = _mm_add_pi16(r0, r2);
		d0 = _mm_madd_pi16(d0, xf0);
		d0 = _mm_srai_pi32(d0, 16);
		ri = _m_to_int(_mm_packs_pi32(d0, _m_mzero[0]));

		*(mlib_s32 *)dstPixelPtr = ri;

		dstPixelPtr += 2;
		x += dx;
		y += dy;
	}

	if (i < size) {
		CALC_PTR(0)

		    s0 = *(__m64 *) (sPtr0);
		s1 = *(__m64 *) (sPtr0 + srcStride);
		s2 = *(__m64 *) (sPtr0 + 2 * srcStride);

		yf0 = fptr0[0];
		yf1 = fptr0[1];
		yf2 = fptr0[2];

		r0 = _mm_mulhi_pi16(s0, yf0);
		r1 = _mm_mulhi_pi16(s1, yf1);
		r2 = _mm_mulhi_pi16(s2, yf2);
		r0 = _mm_add_pi16(r0, r1);
		d0 = _mm_add_pi16(r0, r2);
		d0 = _mm_madd_pi16(d0, xf0);
		d0 = _mm_srai_pi32(d0, 16);
		ri = _m_to_int(_mm_packs_pi32(d0, _m_mzero[0]));

		*(mlib_s32 *)dstPixelPtr = ri;
	}
	_mm_empty();
}

/* ******************************************************************** */

void
mlib_m_ImageAffineTableLine_s16nw_2_2_1(
    __m64 * buff,
    __m64 * filterX,
    __m64 * filterY,
    mlib_u8 **lineAddr,
    mlib_affine_workspace * ws)
{
	DECLAREVAR mlib_u8 *sPtr1;
	__m64 *fptr1, xf1;
	__m64 yf0, yf1, s0, s1;
	__m64 _m_mzero[1] = { 0 };
	__m64 mask = _m_from_int(0xFFFFFFFF);

	dstPixelPtr = (mlib_s16 *)buff;

	for (i = 0; i < size - 1; i += 2) {
		CALC_PTR(0)
		    x += dx;
		y += dy;
		CALC_PTR(1)

		    s0 = _mm_or_si64(_mm_slli_si64((*(__m64 *) (sPtr1)), 32),
		    _mm_and_si64((*(__m64 *) (sPtr0)), mask));

		s1 = _mm_or_si64(_mm_slli_si64((*(__m64 *) (sPtr1 + srcStride)),
		    32), _mm_and_si64((*(__m64 *) (sPtr0 + srcStride)), mask));

		yf0 = _mm_unpackhi_pi32(fptr0[0], fptr1[0]);
		yf1 = _mm_unpackhi_pi32(fptr0[1], fptr1[1]);

		xf0 = _mm_unpacklo_pi32(xf0, xf1);

		r0 = _mm_mulhi_pi16(s0, yf0);
		r1 = _mm_mulhi_pi16(s1, yf1);
		d0 = _mm_add_pi16(r0, r1);
		d0 = _mm_madd_pi16(d0, xf0);
		d0 = _mm_srai_pi32(d0, 16);
		ri = _m_to_int(_mm_packs_pi32(d0, _m_mzero[0]));

		*(mlib_s32 *)dstPixelPtr = ri;

		dstPixelPtr += 2;
		x += dx;
		y += dy;
	}

	if (i < size) {
		CALC_PTR(0)

		    s0 = *(__m64 *) (sPtr0);
		s1 = *(__m64 *) (sPtr0 + srcStride);

		yf0 = fptr0[0];
		yf1 = fptr0[1];

		r0 = _mm_mulhi_pi16(s0, yf0);
		r1 = _mm_mulhi_pi16(s1, yf1);
		d0 = _mm_add_pi16(r0, r1);
		d0 = _mm_madd_pi16(d0, xf0);
		d0 = _mm_srai_pi32(d0, 16);
		ri = _m_to_int(_mm_packs_pi32(d0, _m_mzero[0]));

		*(mlib_s32 *)dstPixelPtr = ri;
	}
	_mm_empty();
}

/* ******************************************************************** */

void
mlib_m_ImageAffineTableLine_u16nw_4_4_1(
    __m64 * buff,
    __m64 * filterX,
    __m64 * filterY,
    mlib_u8 **lineAddr,
    mlib_affine_workspace * ws)
{
	DECLAREVAR __m64 _m_mzero[1] = {
	0};
	__m64 r2, r3;

	dstPixelPtr = (mlib_s16 *)buff;

	for (i = 0; i < size; i++) {
		CALC_PTR(0)
		    r0 = _mm_srli_pi16(*(__m64 *) (sPtr0), 1);
		r1 = _mm_srli_pi16(*(__m64 *) (sPtr0 + srcStride), 1);
		r2 = _mm_srli_pi16(*(__m64 *) (sPtr0 + 2 * srcStride), 1);
		r3 = _mm_srli_pi16(*(__m64 *) (sPtr0 + 3 * srcStride), 1);
		r0 = _mm_mulhi_pi16(r0, fptr0[0]);
		r1 = _mm_mulhi_pi16(r1, fptr0[1]);
		r2 = _mm_mulhi_pi16(r2, fptr0[2]);
		r3 = _mm_mulhi_pi16(r3, fptr0[3]);
		r0 = _mm_add_pi16(r0, r1);
		r2 = _mm_add_pi16(r2, r3);
		d0 = _mm_add_pi16(r0, r2);
		d0 = _mm_madd_pi16(d0, xf0);
		d0 = _mm_add_pi32(d0, _mm_srli_si64(d0, 32));
		d0 = _mm_srai_pi32(d0, 16);
		ri = _m_to_int(_mm_packs_pi32(d0, _m_mzero[0]));
		*dstPixelPtr = ri;
		dstPixelPtr++;
		x += dx;
		y += dy;
	}

	_mm_empty();
}

/* ******************************************************************** */

void
mlib_m_ImageAffineTableLine_u16nw_3_4_1(
    __m64 * buff,
    __m64 * filterX,
    __m64 * filterY,
    mlib_u8 **lineAddr,
    mlib_affine_workspace * ws)
{
	DECLAREVAR __m64 _m_mzero[1] = {
	0};
	__m64 r2;

	dstPixelPtr = (mlib_s16 *)buff;

	for (i = 0; i < size; i++) {
		CALC_PTR(0)
		    r0 = _mm_srli_pi16(*(__m64 *) (sPtr0), 1);
		r1 = _mm_srli_pi16(*(__m64 *) (sPtr0 + srcStride), 1);
		r2 = _mm_srli_pi16(*(__m64 *) (sPtr0 + 2 * srcStride), 1);
		r0 = _mm_mulhi_pi16(r0, fptr0[0]);
		r1 = _mm_mulhi_pi16(r1, fptr0[1]);
		r2 = _mm_mulhi_pi16(r2, fptr0[2]);
		r0 = _mm_add_pi16(r0, r1);
		d0 = _mm_add_pi16(r0, r2);
		d0 = _mm_madd_pi16(d0, xf0);
		d0 = _mm_add_pi32(d0, _mm_srli_si64(d0, 32));
		d0 = _mm_srai_pi32(d0, 16);
		ri = _m_to_int(_mm_packs_pi32(d0, _m_mzero[0]));
		*dstPixelPtr = ri;
		dstPixelPtr++;
		x += dx;
		y += dy;
	}

	_mm_empty();
}

/* ******************************************************************** */

void
mlib_m_ImageAffineTableLine_u16nw_2_4_1(
    __m64 * buff,
    __m64 * filterX,
    __m64 * filterY,
    mlib_u8 **lineAddr,
    mlib_affine_workspace * ws)
{
	DECLAREVAR __m64 _m_mzero[1] = {
	0};

	dstPixelPtr = (mlib_s16 *)buff;

	for (i = 0; i < size; i++) {
		CALC_PTR(0)
		    r0 = _mm_srli_pi16(*(__m64 *) (sPtr0), 1);
		r1 = _mm_srli_pi16(*(__m64 *) (sPtr0 + srcStride), 1);
		r0 = _mm_mulhi_pi16(r0, fptr0[0]);
		r1 = _mm_mulhi_pi16(r1, fptr0[1]);
		d0 = _mm_add_pi16(r0, r1);
		d0 = _mm_madd_pi16(d0, xf0);
		d0 = _mm_add_pi32(d0, _mm_srli_si64(d0, 32));
		d0 = _mm_srai_pi32(d0, 16);
		ri = _m_to_int(_mm_packs_pi32(d0, _m_mzero[0]));
		*dstPixelPtr = ri;
		dstPixelPtr++;
		x += dx;
		y += dy;
	}

	_mm_empty();
}

/* ******************************************************************** */

void
mlib_m_ImageAffineTableLine_u16nw_4_2_1(
    __m64 * buff,
    __m64 * filterX,
    __m64 * filterY,
    mlib_u8 **lineAddr,
    mlib_affine_workspace * ws)
{
	DECLAREVAR mlib_u8 *sPtr1;
	__m64 *fptr1, xf1;
	__m64 s0, s1, s2, s3;
	__m64 yf0, yf1, yf2, yf3, r2, r3;
	__m64 _m_mzero[1] = { 0 };
	__m64 mask = _m_from_int(0xFFFFFFFF);

	dstPixelPtr = (mlib_s16 *)buff;

	for (i = 0; i < size - 1; i += 2) {
		CALC_PTR(0)
		    x += dx;
		y += dy;
		CALC_PTR(1)

		    s0 = _mm_or_si64(_mm_slli_si64((*(__m64 *) (sPtr1)), 32),
		    _mm_and_si64((*(__m64 *) (sPtr0)), mask));

		s1 = _mm_or_si64(_mm_slli_si64((*(__m64 *) (sPtr1 + srcStride)),
		    32), _mm_and_si64((*(__m64 *) (sPtr0 + srcStride)), mask));

		s2 = _mm_or_si64(_mm_slli_si64((*(__m64 *) (sPtr1 +
		    2 * srcStride)), 32),
		    _mm_and_si64((*(__m64 *) (sPtr0 + 2 * srcStride)), mask));

		s3 = _mm_or_si64(_mm_slli_si64((*(__m64 *) (sPtr1 +
		    3 * srcStride)), 32),
		    _mm_and_si64((*(__m64 *) (sPtr0 + 3 * srcStride)), mask));

		s0 = _mm_srli_pi16(s0, 1);
		s1 = _mm_srli_pi16(s1, 1);
		s2 = _mm_srli_pi16(s2, 1);
		s3 = _mm_srli_pi16(s3, 1);

		yf0 = _mm_unpackhi_pi32(fptr0[0], fptr1[0]);
		yf1 = _mm_unpackhi_pi32(fptr0[1], fptr1[1]);
		yf2 = _mm_unpackhi_pi32(fptr0[2], fptr1[2]);
		yf3 = _mm_unpackhi_pi32(fptr0[3], fptr1[3]);

		xf0 = _mm_unpacklo_pi32(xf0, xf1);

		r0 = _mm_mulhi_pi16(s0, yf0);
		r1 = _mm_mulhi_pi16(s1, yf1);
		r2 = _mm_mulhi_pi16(s2, yf2);
		r3 = _mm_mulhi_pi16(s3, yf3);
		r0 = _mm_add_pi16(r0, r1);
		r2 = _mm_add_pi16(r2, r3);
		d0 = _mm_add_pi16(r0, r2);
		d0 = _mm_madd_pi16(d0, xf0);
		d0 = _mm_srai_pi32(d0, 16);
		ri = _m_to_int(_mm_packs_pi32(d0, _m_mzero[0]));

		*(mlib_s32 *)dstPixelPtr = ri;

		dstPixelPtr += 2;
		x += dx;
		y += dy;
	}

	if (i < size) {
		CALC_PTR(0)

		    s0 = *(__m64 *) (sPtr0);
		s1 = *(__m64 *) (sPtr0 + srcStride);
		s2 = *(__m64 *) (sPtr0 + 2 * srcStride);
		s3 = *(__m64 *) (sPtr0 + 3 * srcStride);

		s0 = _mm_srli_pi16(s0, 1);
		s1 = _mm_srli_pi16(s1, 1);
		s2 = _mm_srli_pi16(s2, 1);
		s3 = _mm_srli_pi16(s3, 1);

		yf0 = fptr0[0];
		yf1 = fptr0[1];
		yf2 = fptr0[2];
		yf3 = fptr0[3];

		r0 = _mm_mulhi_pi16(s0, yf0);
		r1 = _mm_mulhi_pi16(s1, yf1);
		r2 = _mm_mulhi_pi16(s2, yf2);
		r3 = _mm_mulhi_pi16(s3, yf3);
		r0 = _mm_add_pi16(r0, r1);
		r2 = _mm_add_pi16(r2, r3);
		d0 = _mm_add_pi16(r0, r2);
		d0 = _mm_madd_pi16(d0, xf0);
		d0 = _mm_srai_pi32(d0, 16);
		ri = _m_to_int(_mm_packs_pi32(d0, _m_mzero[0]));

		*(mlib_s32 *)dstPixelPtr = ri;
	}
	_mm_empty();
}

/* ******************************************************************** */

void
mlib_m_ImageAffineTableLine_u16nw_3_2_1(
    __m64 * buff,
    __m64 * filterX,
    __m64 * filterY,
    mlib_u8 **lineAddr,
    mlib_affine_workspace * ws)
{
	DECLAREVAR mlib_u8 *sPtr1;
	__m64 *fptr1, xf1;
	__m64 s0, s1, s2;
	__m64 yf0, yf1, yf2, r2;
	__m64 _m_mzero[1] = { 0 };
	__m64 mask = _m_from_int(0xFFFFFFFF);

	dstPixelPtr = (mlib_s16 *)buff;

	for (i = 0; i < size - 1; i += 2) {
		CALC_PTR(0)
		    x += dx;
		y += dy;
		CALC_PTR(1)

		    s0 = _mm_or_si64(_mm_slli_si64((*(__m64 *) (sPtr1)), 32),
		    _mm_and_si64((*(__m64 *) (sPtr0)), mask));

		s1 = _mm_or_si64(_mm_slli_si64((*(__m64 *) (sPtr1 + srcStride)),
		    32), _mm_and_si64((*(__m64 *) (sPtr0 + srcStride)), mask));

		s2 = _mm_or_si64(_mm_slli_si64((*(__m64 *) (sPtr1 +
		    2 * srcStride)), 32),
		    _mm_and_si64((*(__m64 *) (sPtr0 + 2 * srcStride)), mask));

		s0 = _mm_srli_pi16(s0, 1);
		s1 = _mm_srli_pi16(s1, 1);
		s2 = _mm_srli_pi16(s2, 1);

		yf0 = _mm_unpackhi_pi32(fptr0[0], fptr1[0]);
		yf1 = _mm_unpackhi_pi32(fptr0[1], fptr1[1]);
		yf2 = _mm_unpackhi_pi32(fptr0[2], fptr1[2]);

		xf0 = _mm_unpacklo_pi32(xf0, xf1);

		r0 = _mm_mulhi_pi16(s0, yf0);
		r1 = _mm_mulhi_pi16(s1, yf1);
		r2 = _mm_mulhi_pi16(s2, yf2);
		r0 = _mm_add_pi16(r0, r1);
		d0 = _mm_add_pi16(r0, r2);
		d0 = _mm_madd_pi16(d0, xf0);
		d0 = _mm_srai_pi32(d0, 16);
		ri = _m_to_int(_mm_packs_pi32(d0, _m_mzero[0]));

		*(mlib_s32 *)dstPixelPtr = ri;

		dstPixelPtr += 2;
		x += dx;
		y += dy;
	}

	if (i < size) {
		CALC_PTR(0)

		    s0 = *(__m64 *) (sPtr0);
		s1 = *(__m64 *) (sPtr0 + srcStride);
		s2 = *(__m64 *) (sPtr0 + 2 * srcStride);

		s0 = _mm_srli_pi16(s0, 1);
		s1 = _mm_srli_pi16(s1, 1);
		s2 = _mm_srli_pi16(s2, 1);

		yf0 = fptr0[0];
		yf1 = fptr0[1];
		yf2 = fptr0[2];

		r0 = _mm_mulhi_pi16(s0, yf0);
		r1 = _mm_mulhi_pi16(s1, yf1);
		r2 = _mm_mulhi_pi16(s2, yf2);
		r0 = _mm_add_pi16(r0, r1);
		d0 = _mm_add_pi16(r0, r2);
		d0 = _mm_madd_pi16(d0, xf0);
		d0 = _mm_srai_pi32(d0, 16);
		ri = _m_to_int(_mm_packs_pi32(d0, _m_mzero[0]));

		*(mlib_s32 *)dstPixelPtr = ri;
	}
	_mm_empty();
}

/* ******************************************************************** */

void
mlib_m_ImageAffineTableLine_u16nw_2_2_1(
    __m64 * buff,
    __m64 * filterX,
    __m64 * filterY,
    mlib_u8 **lineAddr,
    mlib_affine_workspace * ws)
{
	DECLAREVAR mlib_u8 *sPtr1;
	__m64 *fptr1, xf1;
	__m64 yf0, yf1, s0, s1;
	__m64 _m_mzero[1] = { 0 };
	__m64 mask = _m_from_int(0xFFFFFFFF);

	dstPixelPtr = (mlib_s16 *)buff;

	for (i = 0; i < size - 1; i += 2) {
		CALC_PTR(0)
		    x += dx;
		y += dy;
		CALC_PTR(1)

		    s0 = _mm_or_si64(_mm_slli_si64((*(__m64 *) (sPtr1)), 32),
		    _mm_and_si64((*(__m64 *) (sPtr0)), mask));

		s1 = _mm_or_si64(_mm_slli_si64((*(__m64 *) (sPtr1 + srcStride)),
		    32), _mm_and_si64((*(__m64 *) (sPtr0 + srcStride)), mask));

		s0 = _mm_srli_pi16(s0, 1);
		s1 = _mm_srli_pi16(s1, 1);

		yf0 = _mm_unpackhi_pi32(fptr0[0], fptr1[0]);
		yf1 = _mm_unpackhi_pi32(fptr0[1], fptr1[1]);

		xf0 = _mm_unpacklo_pi32(xf0, xf1);

		r0 = _mm_mulhi_pi16(s0, yf0);
		r1 = _mm_mulhi_pi16(s1, yf1);
		d0 = _mm_add_pi16(r0, r1);
		d0 = _mm_madd_pi16(d0, xf0);
		d0 = _mm_srai_pi32(d0, 16);
		ri = _m_to_int(_mm_packs_pi32(d0, _m_mzero[0]));

		*(mlib_s32 *)dstPixelPtr = ri;

		dstPixelPtr += 2;
		x += dx;
		y += dy;
	}

	if (i < size) {
		CALC_PTR(0)

		    s0 = *(__m64 *) (sPtr0);
		s1 = *(__m64 *) (sPtr0 + srcStride);

		s0 = _mm_srli_pi16(s0, 1);
		s1 = _mm_srli_pi16(s1, 1);

		yf0 = fptr0[0];
		yf1 = fptr0[1];

		r0 = _mm_mulhi_pi16(s0, yf0);
		r1 = _mm_mulhi_pi16(s1, yf1);
		d0 = _mm_add_pi16(r0, r1);
		d0 = _mm_madd_pi16(d0, xf0);
		d0 = _mm_srai_pi32(d0, 16);
		ri = _m_to_int(_mm_packs_pi32(d0, _m_mzero[0]));

		*(mlib_s32 *)dstPixelPtr = ri;
	}
	_mm_empty();
}

/* ******************************************************************** */
