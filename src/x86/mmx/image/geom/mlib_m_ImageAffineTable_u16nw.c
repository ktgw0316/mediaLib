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

#pragma ident	"@(#)mlib_m_ImageAffineTable_u16nw.c	9.4	07/11/05 SMI"

/*
 * FUNCTION
 *      Internal functions for mlib_ImageAffineTable.
 */

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

#define	FLT_BITS	1
/* ******************************************************************** */
#define	PARAMS_INT                                                 \
	__m64 *buff, __m64 * filterX, __m64 * filterY,             \
	    const mlib_u8 **lineAddr, mlib_affine_workspace * ws
/* ******************************************************************** */

void mlib_m_ImageAffineTableLine_u16nw_4_4_1(
    PARAMS_INT);
void mlib_m_ImageAffineTableLine_u16nw_4_2_1(
    PARAMS_INT);
void mlib_m_ImageAffineTableLine_u16nw_3_4_1(
    PARAMS_INT);
void mlib_m_ImageAffineTableLine_u16nw_3_2_1(
    PARAMS_INT);
void mlib_m_ImageAffineTableLine_u16nw_2_4_1(
    PARAMS_INT);
void mlib_m_ImageAffineTableLine_u16nw_2_2_1(
    PARAMS_INT);

void mlib_m_ImageAffineTableLine_u16nw_4_4_2(
    PARAMS_INT);
void mlib_m_ImageAffineTableLine_u16nw_4_2_2(
    PARAMS_INT);
void mlib_m_ImageAffineTableLine_u16nw_3_4_2(
    PARAMS_INT);
void mlib_m_ImageAffineTableLine_u16nw_3_2_2(
    PARAMS_INT);
void mlib_m_ImageAffineTableLine_u16nw_2_4_2(
    PARAMS_INT);
void mlib_m_ImageAffineTableLine_u16nw_2_2_2(
    PARAMS_INT);

void mlib_m_ImageAffineTableLine_u16nw_4_4_3(
    PARAMS_INT);
void mlib_m_ImageAffineTableLine_u16nw_4_3_3(
    PARAMS_INT);
void mlib_m_ImageAffineTableLine_u16nw_4_2_3(
    PARAMS_INT);
void mlib_m_ImageAffineTableLine_u16nw_3_4_3(
    PARAMS_INT);
void mlib_m_ImageAffineTableLine_u16nw_3_3_3(
    PARAMS_INT);
void mlib_m_ImageAffineTableLine_u16nw_3_2_3(
    PARAMS_INT);
void mlib_m_ImageAffineTableLine_u16nw_2_4_3(
    PARAMS_INT);
void mlib_m_ImageAffineTableLine_u16nw_2_3_3(
    PARAMS_INT);
void mlib_m_ImageAffineTableLine_u16nw_2_2_3(
    PARAMS_INT);

void mlib_m_ImageAffineTableLine_u16nw_4_4_4(
    PARAMS_INT);
void mlib_m_ImageAffineTableLine_u16nw_4_3_4(
    PARAMS_INT);
void mlib_m_ImageAffineTableLine_u16nw_4_2_4(
    PARAMS_INT);
void mlib_m_ImageAffineTableLine_u16nw_3_4_4(
    PARAMS_INT);
void mlib_m_ImageAffineTableLine_u16nw_3_3_4(
    PARAMS_INT);
void mlib_m_ImageAffineTableLine_u16nw_3_2_4(
    PARAMS_INT);
void mlib_m_ImageAffineTableLine_u16nw_2_4_4(
    PARAMS_INT);
void mlib_m_ImageAffineTableLine_u16nw_2_3_4(
    PARAMS_INT);
void mlib_m_ImageAffineTableLine_u16nw_2_2_4(
    PARAMS_INT);

/* ******************************************************************** */
#define	BUFF_SIZE	256
/* ******************************************************************** */
#define	CALL_PARAMS                                                       \
	dstData, lineAddr, leftEdges, rightEdges, xStarts, yStarts, ws,   \
	    table
/* ******************************************************************** */
#define	CALL_PARAMS_INT                                          \
	buff, filterX + offx, filterY + offy, lineAddr + l, ws
/* ******************************************************************** */
#define	DECLAREVAR                                              \
	mlib_s32 *sides = ws->sides;                            \
	mlib_s32 affine_mask = ws->affine_mask;                 \
	mlib_s32 xLeft, xRight, x0, y0;                         \
	mlib_s32 yStart = ws->yStart;                           \
	mlib_s32 yFinish = ws->yFinish;                         \
	mlib_s32 max_xsize = ws->max_xsize;                     \
	mlib_s32 dstStride = ws->dstStride;                     \
	mlib_s32 x_shift = ws->x_shift0;                        \
	mlib_s32 y_shift = ws->y_shift;                         \
	mlib_s32 xf_shift, xf_mask, yf_shift, yf_mask;          \
	mlib_s32 size, size1;                                   \
	mlib_s16 *dstPixelPtr;                                  \
	mlib_s32 m = table->width;                              \
	mlib_s32 n = table->height;                             \
	mlib_s32 xf_bits = table->vis_width_bits;               \
	mlib_s32 yf_bits = table->vis_height_bits;              \
	mlib_s32 subsampleBitsH = table->subsampleBitsH;        \
	mlib_s32 subsampleBitsV = table->subsampleBitsV;        \
	__m64 *filterX, *filterY;                               \
	mlib_s32 i, j, l, kw, kh, off, offx, offy;              \
	mlib_s32 buff_size;                                     \
	__m64 *ddst, *dp, tail;

/* ******************************************************************** */

#define	DIST_BITS()                                                        \
	xf_shift = x_shift - subsampleBitsH - xf_bits - FLT_BITS;          \
	xf_mask = (((1 << subsampleBitsH) - 1) << (xf_bits + FLT_BITS));   \
	yf_shift = y_shift - subsampleBitsV - yf_bits - FLT_BITS;          \
	yf_mask = (((1 << subsampleBitsV) - 1) << (yf_bits + FLT_BITS));

/* ******************************************************************** */

#define	CLIP(NCHAN)                                             \
	dstData += dstStride;                                   \
	xLeft = leftEdges[j];                                   \
	xRight = rightEdges[j];                                 \
	if (xLeft > xRight)                                     \
	    continue;                                           \
	x0 = xStarts[j];                                        \
	y0 = yStarts[j];                                        \
	if (affine_mask != 0) {                                 \
	    ws->dx = sides[j * 2 + 2];                          \
	    ws->dy = sides[j * 2 + 3];                          \
	}                                                       \
	dstPixelPtr = (mlib_s16 *)dstData + NCHAN * xLeft;      \
	size = xRight - xLeft + 1;

/* ******************************************************************** */

mlib_status
mlib_m_ImageAffineTable_u16nw_1(
    PARAMS_NW)
{
	DECLAREVAR __m64 buff_local[BUFF_SIZE], *buff = buff_local, *buff_acc;
	mlib_s32 shift;
	__m64 dsat, dsat1, mask_8000;

	buff_size = (max_xsize + 3) >> 2;
	if (2 * (buff_size + 2) > BUFF_SIZE)
		buff = mlib_malloc(2 * (buff_size + 2) * sizeof (__m64));
	if (buff == NULL)
		return (MLIB_FAILURE);

	buff_acc = buff + buff_size;

	filterX = (__m64 *) table->dataH_s16_1;
	filterY = (__m64 *) table->dataV_s16_1;

	shift = table->shift_vis_affine;
	shift -= 33;

	if (shift > 0) {
		dsat =
		    mmx_from_int_dup((1 << (shift + 15)) | (1 << (shift - 1)));
	} else {
		dsat = mmx_from_int_dup(0);
	}

	shift += 16;

	dsat1 = _m_from_int(0x8000);
	dsat1 = _m_por(dsat1, _mm_slli_si64(dsat1, 32));
	mask_8000 = _m_from_int(0x80008000);
	mask_8000 = _mm_or_si64(mask_8000, _mm_slli_si64(mask_8000, 32));

	DIST_BITS()

	ws->xf_shift = xf_shift;
	ws->xf_mask = xf_mask;
	ws->yf_shift = yf_shift;
	ws->yf_mask = yf_mask;

	for (j = yStart; j <= yFinish; j++) {
		CLIP(1)

		size1 = (size + 3) >> 2;

		for (i = 0; i < size1; i++) {
			buff_acc[i] = dsat;
		}

/* move to kernel center */
		x0 -= ws->x_move;
		y0 -= ws->y_move;

		ws->size = size;
		ws->x0 = x0;
		ws->y0 = y0;

		offy = 0;
/* kernel lines */
		for (l = 0; l < n; l += kh) {
			kh = n - l;

			if ((kh >= 4) && (kh != 5))
				kh = 4;
			else if (kh >= 3)
				kh = 3;

			offx = 0;
/* offset in current kernel line */
			for (off = 0; off < m; off += kw) {

				ws->x0 = x0 + (off << x_shift);

				kw = m - off;

				if ((kw >= 4) && (kw != 5))
					kw = 4;
				else if (kw >= 3)
					kw = 3;

				_mm_empty();

				if (kh == 4) {
				    if (kw == 4)
					mlib_m_ImageAffineTableLine_u16nw_4_4_1
					    (CALL_PARAMS_INT);
				    else if (kw == 3)
					mlib_m_ImageAffineTableLine_u16nw_4_4_1
					    (CALL_PARAMS_INT);
				    else if (kw == 2)
					mlib_m_ImageAffineTableLine_u16nw_4_2_1
					    (CALL_PARAMS_INT);
				} else if (kh == 3) {
				    if (kw == 4)
					mlib_m_ImageAffineTableLine_u16nw_3_4_1
					    (CALL_PARAMS_INT);
				    else if (kw == 3)
					mlib_m_ImageAffineTableLine_u16nw_3_4_1
					    (CALL_PARAMS_INT);
				    else if (kw == 2)
					mlib_m_ImageAffineTableLine_u16nw_3_2_1
					    (CALL_PARAMS_INT);
				} else {	/* if (kh == 2) */

				    if (kw == 4)
					mlib_m_ImageAffineTableLine_u16nw_2_4_1
					    (CALL_PARAMS_INT);
				    else if (kw == 3)
					mlib_m_ImageAffineTableLine_u16nw_2_4_1
					    (CALL_PARAMS_INT);
				    else if (kw == 2)
					mlib_m_ImageAffineTableLine_u16nw_2_2_1
					    (CALL_PARAMS_INT);
				}

				offx++;

				for (i = 0; i < size1; i++) {
					buff_acc[i] =
					    _mm_add_pi16(buff_acc[i], buff[i]);
				}
			}
			offy += 4;
		}

		ddst = (__m64 *) buff_acc;
		dp = (__m64 *) dstPixelPtr;

		tail = *(__m64 *) (dstPixelPtr + size);

		for (i = 0; i < size; i += 4) {
			__m64 d0, d1;

			d0 = _mm_srai_pi32(_mm_unpacklo_pi16(_m_zero, ddst[0]),
			    shift);
			d1 = _mm_srai_pi32(_mm_unpackhi_pi16(_m_zero, ddst[0]),
			    shift);
			d0 = _mm_sub_pi32(d0, dsat1);
			d1 = _mm_sub_pi32(d1, dsat1);
			*dp = _mm_xor_si64(_mm_packs_pi32(d0, d1), mask_8000);
			dp++;
			ddst++;
		}

		*(__m64 *) (dstPixelPtr + size) = tail;
	}

	_mm_empty();

	if (buff != buff_local)
		mlib_free(buff);
	return (MLIB_SUCCESS);
}

/* ******************************************************************** */

mlib_status
mlib_m_ImageAffineTable_u16nw_2(
    PARAMS_NW)
{
	DECLAREVAR __m64 buff_local[BUFF_SIZE], *buff = buff_local, *buff_acc;
	mlib_s32 shift;
	__m64 dsat, dsat1, mask_8000;

	buff_size = (max_xsize * 2 + 3) >> 2;
	if (2 * (buff_size + 2) > BUFF_SIZE)
		buff = mlib_malloc(2 * (buff_size + 2) * sizeof (__m64));
	if (buff == NULL)
		return (MLIB_FAILURE);

	buff_acc = buff + buff_size;

	filterX = (__m64 *) table->dataH_s16_3;
	filterY = (__m64 *) table->dataV_s16_1;

	shift = table->shift_vis_affine;
	shift -= 33;

	if (shift > 0) {
		dsat =
		    mmx_from_int_dup((1 << (shift + 15)) | (1 << (shift - 1)));
	} else {
		dsat = mmx_from_int_dup(0);
	}

	shift += 16;

	dsat1 = _m_from_int(0x8000);
	dsat1 = _m_por(dsat1, _mm_slli_si64(dsat1, 32));
	mask_8000 = _m_from_int(0x80008000);
	mask_8000 = _mm_or_si64(mask_8000, _mm_slli_si64(mask_8000, 32));

	DIST_BITS()

	    x_shift -= 1;

	ws->x_shift = x_shift;
	ws->xf_shift = xf_shift;
	ws->xf_mask = xf_mask;
	ws->yf_shift = yf_shift;
	ws->yf_mask = yf_mask;

	for (j = yStart; j <= yFinish; j++) {
		CLIP(2)

		    size1 = (size * 2 + 3) >> 2;

		for (i = 0; i < size1; i++) {
			buff_acc[i] = dsat;
		}

/* move to kernel center */
		x0 -= ws->x_move;
		y0 -= ws->y_move;

		ws->size = size;
		ws->x0 = x0;
		ws->y0 = y0;

		offy = 0;
/* kernel lines */
		for (l = 0; l < n; l += kh) {
			kh = n - l;

			if ((kh >= 4) && (kh != 5))
				kh = 4;
			else if (kh >= 3)
				kh = 3;

			offx = 0;
/* offset in current kernel line */
			for (off = 0; off < m; off += kw) {

				ws->x0 = x0 + (off << (x_shift + 1));

				kw = m - off;

				if ((kw >= 4) && (kw != 5))
					kw = 4;
				else if (kw >= 3)
					kw = 3;

				_mm_empty();

				if (kh == 4) {
				    if (kw == 4)
					mlib_m_ImageAffineTableLine_u16nw_4_4_2
					    (CALL_PARAMS_INT);
				    else if (kw == 3)
					mlib_m_ImageAffineTableLine_u16nw_4_4_2
					    (CALL_PARAMS_INT);
				    else if (kw == 2)
					mlib_m_ImageAffineTableLine_u16nw_4_2_2
					    (CALL_PARAMS_INT);
				} else if (kh == 3) {
				    if (kw == 4)
					mlib_m_ImageAffineTableLine_u16nw_3_4_2
					    (CALL_PARAMS_INT);
				    else if (kw == 3)
					mlib_m_ImageAffineTableLine_u16nw_3_4_2
					    (CALL_PARAMS_INT);
				    else if (kw == 2)
					mlib_m_ImageAffineTableLine_u16nw_3_2_2
						    (CALL_PARAMS_INT);
				} else {	/* if (kh == 2) */

				    if (kw == 4)
					mlib_m_ImageAffineTableLine_u16nw_2_4_2
					    (CALL_PARAMS_INT);
				    else if (kw == 3)
					mlib_m_ImageAffineTableLine_u16nw_2_4_2
					    (CALL_PARAMS_INT);
				    else if (kw == 2)
					mlib_m_ImageAffineTableLine_u16nw_2_2_2
					    (CALL_PARAMS_INT);
				}

				offx += 2;

				for (i = 0; i < size1; i++) {
					buff_acc[i] =
					    _mm_add_pi16(buff_acc[i], buff[i]);
				}
			}
			offy += 4;
		}

		size *= 2;

		ddst = (__m64 *) buff_acc;
		dp = (__m64 *) dstPixelPtr;

		tail = *(__m64 *) (dstPixelPtr + size);

		for (i = 0; i < size; i += 4) {
			__m64 d0, d1;

			d0 = _mm_srai_pi32(_mm_unpacklo_pi16(_m_zero, ddst[0]),
			    shift);
			d1 = _mm_srai_pi32(_mm_unpackhi_pi16(_m_zero, ddst[0]),
			    shift);
			d0 = _mm_sub_pi32(d0, dsat1);
			d1 = _mm_sub_pi32(d1, dsat1);
			*dp = _mm_xor_si64(_mm_packs_pi32(d0, d1), mask_8000);
			dp++;
			ddst++;
		}

		*(__m64 *) (dstPixelPtr + size) = tail;
	}

	_mm_empty();

	if (buff != buff_local)
		mlib_free(buff);
	return (MLIB_SUCCESS);
}

/* ******************************************************************** */

mlib_status
mlib_m_ImageAffineTable_u16nw_3(
    PARAMS_NW)
{
	DECLAREVAR __m64 buff_local[BUFF_SIZE], *buff = buff_local, *buff_acc;
	mlib_s32 shift;
	__m64 dsat, dsat1, mask_8000;

	buff_size = ((max_xsize * 3 + 3) >> 2) + 1;
	if (2 * (buff_size + 2) > BUFF_SIZE)
		buff = mlib_malloc(2 * (buff_size + 2) * sizeof (__m64));
	if (buff == NULL)
		return (MLIB_FAILURE);

	buff_acc = buff + buff_size;

	filterX = (__m64 *) table->dataH_s16_4;
	filterY = (__m64 *) table->dataV_s16_1;

	shift = table->shift_vis_affine;
	shift -= 33;

	if (shift > 0) {
		dsat =
		    mmx_from_int_dup((1 << (shift + 15)) | (1 << (shift - 1)));
	} else {
		dsat = mmx_from_int_dup(0);
	}

	shift += 16;

	dsat1 = _m_from_int(0x8000);
	dsat1 = _m_por(dsat1, _mm_slli_si64(dsat1, 32));
	mask_8000 = _m_from_int(0x80008000);
	mask_8000 = _mm_or_si64(mask_8000, _mm_slli_si64(mask_8000, 32));

	DIST_BITS()

	    ws->xf_shift = xf_shift;
	ws->xf_mask = xf_mask;
	ws->yf_shift = yf_shift;
	ws->yf_mask = yf_mask;

	for (j = yStart; j <= yFinish; j++) {
		CLIP(3)

		    size1 = ((size * 3 + 3) >> 2) + 1;

		for (i = 0; i < size1; i++) {
			buff_acc[i] = dsat;
		}

/* move to kernel center */
		x0 -= ws->x_move;
		y0 -= ws->y_move;

		ws->size = size;
		ws->x0 = x0;
		ws->y0 = y0;

		offy = 0;
/* kernel lines */
		for (l = 0; l < n; l += kh) {
			kh = n - l;

			if ((kh >= 4) && (kh != 5))
				kh = 4;
			else if (kh >= 3)
				kh = 3;

			offx = 0;
/* offset in current kernel line */
			for (off = 0; off < m; off += kw) {

				ws->x0 = x0 + (off << x_shift);

				kw = m - off;

				if ((kw >= 4) && (kw != 5))
					kw = 4;
				else if (kw >= 3)
					kw = 3;

				_mm_empty();

				if (kh == 4) {
				    if (kw == 4)
					mlib_m_ImageAffineTableLine_u16nw_4_4_3
					    (CALL_PARAMS_INT);
				    else if (kw == 3)
					mlib_m_ImageAffineTableLine_u16nw_4_3_3
					    (CALL_PARAMS_INT);
				    else if (kw == 2)
					mlib_m_ImageAffineTableLine_u16nw_4_2_3
					    (CALL_PARAMS_INT);
				} else if (kh == 3) {
				    if (kw == 4)
					mlib_m_ImageAffineTableLine_u16nw_3_4_3
					    (CALL_PARAMS_INT);
				    else if (kw == 3)
					mlib_m_ImageAffineTableLine_u16nw_3_3_3
					    (CALL_PARAMS_INT);
				    else if (kw == 2)
					mlib_m_ImageAffineTableLine_u16nw_3_2_3
					    (CALL_PARAMS_INT);
				} else {	/* if (kh == 2) */

				    if (kw == 4)
					mlib_m_ImageAffineTableLine_u16nw_2_4_3
					    (CALL_PARAMS_INT);
				    else if (kw == 3)
					mlib_m_ImageAffineTableLine_u16nw_2_3_3
					    (CALL_PARAMS_INT);
				    else if (kw == 2)
					mlib_m_ImageAffineTableLine_u16nw_2_2_3
					    (CALL_PARAMS_INT);
				}

				offx += 4;

				for (i = 0; i < size1; i++) {
					buff_acc[i] =
					    _mm_add_pi16(buff_acc[i], buff[i]);
				}
			}
			offy += 4;
		}

		size *= 3;

		ddst = (__m64 *) buff_acc;
		dp = (__m64 *) dstPixelPtr;

		tail = *(__m64 *) (dstPixelPtr + size);

		for (i = 0; i < size; i += 4) {
			__m64 d0, d1;

			d0 = _mm_srai_pi32(_mm_unpacklo_pi16(_m_zero, ddst[0]),
			    shift);
			d1 = _mm_srai_pi32(_mm_unpackhi_pi16(_m_zero, ddst[0]),
			    shift);
			d0 = _mm_sub_pi32(d0, dsat1);
			d1 = _mm_sub_pi32(d1, dsat1);
			*dp = _mm_xor_si64(_mm_packs_pi32(d0, d1), mask_8000);
			dp++;
			ddst++;
		}

		*(__m64 *) (dstPixelPtr + size) = tail;
	}

	_mm_empty();

	if (buff != buff_local)
		mlib_free(buff);
	return (MLIB_SUCCESS);
}

/* ******************************************************************** */

mlib_status
mlib_m_ImageAffineTable_u16nw_4(
    PARAMS_NW)
{
	DECLAREVAR __m64 buff_local[BUFF_SIZE], *buff = buff_local, *buff_acc;
	mlib_s32 shift;
	__m64 dsat, dsat1, mask_8000;

	buff_size = max_xsize;
	if (2 * (buff_size + 2) > BUFF_SIZE)
		buff = mlib_malloc(2 * (buff_size + 2) * sizeof (__m64));
	if (buff == NULL)
		return (MLIB_FAILURE);

	buff_acc = buff + buff_size;

	filterX = (__m64 *) table->dataH_s16_4;
	filterY = (__m64 *) table->dataV_s16_1;

	shift = table->shift_vis_affine;
	shift -= 33;

	if (shift > 0) {
		dsat =
		    mmx_from_int_dup((1 << (shift + 15)) | (1 << (shift - 1)));
	} else {
		dsat = mmx_from_int_dup(0);
	}

	shift += 16;

	dsat1 = _m_from_int(0x8000);
	dsat1 = _m_por(dsat1, _mm_slli_si64(dsat1, 32));
	mask_8000 = _m_from_int(0x80008000);
	mask_8000 = _mm_or_si64(mask_8000, _mm_slli_si64(mask_8000, 32));

	DIST_BITS()

	    ws->xf_shift = xf_shift;
	ws->xf_mask = xf_mask;
	ws->yf_shift = yf_shift;
	ws->yf_mask = yf_mask;

	for (j = yStart; j <= yFinish; j++) {
		CLIP(4)

		    size1 = size;

		for (i = 0; i < size1; i++) {
			buff_acc[i] = dsat;
		}

/* move to kernel center */
		x0 -= ws->x_move;
		y0 -= ws->y_move;

		ws->size = size;
		ws->x0 = x0;
		ws->y0 = y0;

		offy = 0;
/* kernel lines */
		for (l = 0; l < n; l += kh) {
			kh = n - l;

			if ((kh >= 4) && (kh != 5))
				kh = 4;
			else if (kh >= 3)
				kh = 3;

			offx = 0;
/* offset in current kernel line */
			for (off = 0; off < m; off += kw) {

				ws->x0 = x0 + (off << x_shift);

				kw = m - off;

				if ((kw >= 4) && (kw != 5))
					kw = 4;
				else if (kw >= 3)
					kw = 3;

				_mm_empty();

				if (kh == 4) {
				    if (kw == 4)
					mlib_m_ImageAffineTableLine_u16nw_4_4_4
					    (CALL_PARAMS_INT);
				    else if (kw == 3)
					mlib_m_ImageAffineTableLine_u16nw_4_3_4
					    (CALL_PARAMS_INT);
				    else if (kw == 2)
					mlib_m_ImageAffineTableLine_u16nw_4_2_4
					    (CALL_PARAMS_INT);
				} else if (kh == 3) {
				    if (kw == 4)
					mlib_m_ImageAffineTableLine_u16nw_3_4_4
					    (CALL_PARAMS_INT);
				    else if (kw == 3)
					mlib_m_ImageAffineTableLine_u16nw_3_3_4
					    (CALL_PARAMS_INT);
				    else if (kw == 2)
					mlib_m_ImageAffineTableLine_u16nw_3_2_4
					    (CALL_PARAMS_INT);
				} else {	/* if (kh == 2) */

				    if (kw == 4)
					mlib_m_ImageAffineTableLine_u16nw_2_4_4
					    (CALL_PARAMS_INT);
				    else if (kw == 3)
					mlib_m_ImageAffineTableLine_u16nw_2_3_4
					    (CALL_PARAMS_INT);
				    else if (kw == 2)
					mlib_m_ImageAffineTableLine_u16nw_2_2_4
					    (CALL_PARAMS_INT);
				}

				offx += 4;

				for (i = 0; i < size1; i++) {
					buff_acc[i] =
					    _mm_add_pi16(buff_acc[i], buff[i]);
				}
			}
			offy += 4;
		}

		size *= 4;

		ddst = (__m64 *) buff_acc;
		dp = (__m64 *) dstPixelPtr;

		tail = *(__m64 *) (dstPixelPtr + size);

		for (i = 0; i < size; i += 4) {
			__m64 d0, d1;

			d0 = _mm_srai_pi32(_mm_unpacklo_pi16(_m_zero, ddst[0]),
			    shift);
			d1 = _mm_srai_pi32(_mm_unpackhi_pi16(_m_zero, ddst[0]),
			    shift);
			d0 = _mm_sub_pi32(d0, dsat1);
			d1 = _mm_sub_pi32(d1, dsat1);
			*dp = _mm_xor_si64(_mm_packs_pi32(d0, d1), mask_8000);
			dp++;
			ddst++;
		}

		*(__m64 *) (dstPixelPtr + size) = tail;
	}

	_mm_empty();

	if (buff != buff_local)
		mlib_free(buff);
	return (MLIB_SUCCESS);
}

/* ******************************************************************** */

mlib_status
mlib_m_ImageAffineTable_u16nw(
    PARAMS_NW)
{
	mlib_status res;

	switch (ws->nchan) {
	case 1:
		res = mlib_m_ImageAffineTable_u16nw_1(CALL_PARAMS);
		break;
	case 2:
		res = mlib_m_ImageAffineTable_u16nw_2(CALL_PARAMS);
		break;
	case 3:
		res = mlib_m_ImageAffineTable_u16nw_3(CALL_PARAMS);
		break;
	case 4:
		res = mlib_m_ImageAffineTable_u16nw_4(CALL_PARAMS);
		break;
	default:
		res = MLIB_FAILURE;
		break;
	}
	return (res);
}

/* ******************************************************************** */
