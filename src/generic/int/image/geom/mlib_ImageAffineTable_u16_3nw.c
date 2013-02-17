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

#pragma ident	"@(#)mlib_ImageAffineTable_u16_3nw.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      Internal functions for mlib_ImageAffineTable.
 */

#ifndef	_NO_LONGLONG

#include <mlib_image.h>
#include <mlib_ImageInterpTable.h>
#include <mlib_ImageAffineTable.h>

/* *********************************************************** */

/*
 *  These defines switch between functions of different data types:
 *  Files mlib_ImageAffineTable_8_3nw.c,   mlib_ImageAffineTable_16_3nw.c,
 *        mlib_ImageAffineTable_u16_3nw.c, mlib_ImageAffineTable_32_3nw.c,
 *        mlib_ImageAffineTable_F32_3nw.c, mlib_ImageAffineTable_D64_3nw.c
 */

#define	THREE_CHAN

#define	FLT_BITS	3

#define	IMG_TYPE	0

#if IMG_TYPE == 0
#ifdef _MSC_VER
#pragma optimize("", off)
#endif /* _MSC_VER */
#endif /* IMG_TYPE == 0 */

/* *********************************************************** */

#define	LINE_FUNC(KERN)                                                 \
	void                                                            \
	mlib_ImageAffineTableLine_u16_3nw_##KERN(FP_TYPE * buff,        \
		FILTER_TYPE * filterX, FILTER_TYPE * filterY,           \
		const mlib_u8 **lineAddr, mlib_affine_workspace * ws)

#include <mlib_i_ImageAffineTable_func.h>

/* *********************************************************** */

mlib_status
mlib_ImageAffineTable_u16_3nw(
	PARAMS_NW)
{
	DECLAREVAR;
	FP_TYPE buff_local[BUFF_SIZE], *buff = buff_local;
	FP_TYPE sat_off = SAT_OFF;

	if (type < 4) {
		if (max_xsize > BUFF_SIZE) {
			buff = __mlib_malloc(max_xsize * sizeof (FP_TYPE));

			if (buff == NULL)
				return (MLIB_FAILURE);
		}
	}
#if FLT_BITS == 2
	filterX = table->dataH_f32;
	filterY = table->dataV_f32;
#else /* FLT_BITS == 2 */
	filterX = table->dataH_d64;
	filterY = table->dataV_d64;
#endif /* FLT_BITS == 2 */

	DIST_BITS();

	ws->x_shift = x_shift;
	ws->xf_shift = xf_shift;
	ws->xf_mask = xf_mask;
	ws->yf_shift = yf_shift;
	ws->yf_mask = yf_mask;

	for (j = yStart; j <= yFinish; j++) {
		old_size = size;
		CLIP(CHAN1);

		if (type < 4) {
/*
 * process by one channel
 */
			b_step = 1;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = old_size; i < size; i++) {
				buff[i] = sat_off;
			}
		} else {

/* mlib_f32 types */

			b_step = nchan;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < size * nchan; i++) {
				dstPixelPtr[i] = (DTYPE) sat_off;
			}
		}

		ws->b_step = b_step;

/*
 * move to kernel center
 */
		x0 -= ws->x_move;
		y0 -= ws->y_move;

		ws->size = size;
		ws->x0 = x0;
		ws->y0 = y0;

		for (k = 0; k < nchan; k++) {
#if IMG_TYPE < 4
			DTYPE *dPtr = dstPixelPtr + k;
#endif /* IMG_TYPE < 4 */

			ws->k = k;

			if (type >= 4) {
				buff = (void *)(dstPixelPtr + k);
			}

			for (l = 0; l < n; l += kh) {

/* kernel lines */

				kh = n - l;

				if (kh >= 4 && (m & 3) == 0)
					kh = 4;
				else if (kh >= 2)
					kh = 2;

				for (off = 0; off < m; off += kw) {

/* offset in current kernel line */

					ws->x0 = x0 + (off << x_shift);

					kw = m - off;

					if (kw > 2 * MAX_KER)
						kw = MAX_KER;
					else if (kw > MAX_KER)
						kw = kw / 2;

					CALL_FUNC(u16)
				}
			}

#if IMG_TYPE < 4
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < size; i++) {
				FP_TYPE val = buff[i] >> (SHIFT+SHIFT);

#if IMG_TYPE < 3 && defined(MLIB_USE_FTOI_CLAMPING)
				mlib_s32 ival;
#endif /* IMG_TYPE < 3 && defined(MLIB_USE_FTOI_CLAMPING) */

				SAT(dPtr[i * nchan], ival, val);

				buff[i] = sat_off;
			}

#endif /* IMG_TYPE < 4 */
		}
	}

	if (type < 4) {
		if (buff != buff_local)
			__mlib_free(buff);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#if IMG_TYPE == 0
#ifdef _MSC_VER
#pragma optimize("", on)
#endif /* _MSC_VER */
#endif /* IMG_TYPE == 0 */

#else /* _NO_LONGLONG */

#include <mlib_image.h>
#include <mlib_ImageInterpTable.h>
#include <mlib_ImageAffineTable.h>

/* *********************************************************** */

/*
 *  These defines switch between functions of different data types:
 *  Files mlib_ImageAffineTable_8_3nw.c,   mlib_ImageAffineTable_16_3nw.c,
 *        mlib_ImageAffineTable_u16_3nw.c, mlib_ImageAffineTable_32_3nw.c,
 *        mlib_ImageAffineTable_F32_3nw.c, mlib_ImageAffineTable_D64_3nw.c
 */

#define	THREE_CHAN

#define	FLT_BITS	3

#define	IMG_TYPE	0

#if IMG_TYPE == 0
#ifdef _MSC_VER
#pragma optimize("", off)
#endif /* _MSC_VER */
#endif /* IMG_TYPE == 0 */

/* *********************************************************** */

#define	LINE_FUNC(KERN)                                                 \
	void                                                            \
	mlib_ImageAffineTableLine_u16_3nw_##KERN(FP_TYPE * buff,        \
		FILTER_TYPE * filterX, FILTER_TYPE * filterY,           \
		const mlib_u8 **lineAddr, mlib_affine_workspace * ws)

#include <mlib_ImageAffineTable_func.h>

/* *********************************************************** */

mlib_status
mlib_ImageAffineTable_u16_3nw(
	PARAMS_NW)
{
	DECLAREVAR;
	FP_TYPE buff_local[BUFF_SIZE], *buff = buff_local;
	FP_TYPE sat_off = SAT_OFF;

	if (type < 4) {
		if (max_xsize > BUFF_SIZE) {
			buff = __mlib_malloc(max_xsize * sizeof (FP_TYPE));

			if (buff == NULL)
				return (MLIB_FAILURE);
		}
	}
#if FLT_BITS == 2
	filterX = table->dataH_f32;
	filterY = table->dataV_f32;
#else /* FLT_BITS == 2 */
	filterX = table->dataH_d64;
	filterY = table->dataV_d64;
#endif /* FLT_BITS == 2 */

	DIST_BITS();

	ws->x_shift = x_shift;
	ws->xf_shift = xf_shift;
	ws->xf_mask = xf_mask;
	ws->yf_shift = yf_shift;
	ws->yf_mask = yf_mask;

	for (j = yStart; j <= yFinish; j++) {
		old_size = size;
		CLIP(CHAN1);

		if (type < 4) {
/*
 * process by one channel
 */
			b_step = 1;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = old_size; i < size; i++) {
				buff[i] = sat_off;
			}
		} else {

/* mlib_f32 types */

			b_step = nchan;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < size * nchan; i++) {
				dstPixelPtr[i] = (DTYPE) sat_off;
			}
		}

		ws->b_step = b_step;

/*
 * move to kernel center
 */
		x0 -= ws->x_move;
		y0 -= ws->y_move;

		ws->size = size;
		ws->x0 = x0;
		ws->y0 = y0;

		for (k = 0; k < nchan; k++) {
#if IMG_TYPE < 4
			DTYPE *dPtr = dstPixelPtr + k;
#endif /* IMG_TYPE < 4 */

			ws->k = k;

			if (type >= 4) {
				buff = (void *)(dstPixelPtr + k);
			}

			for (l = 0; l < n; l += kh) {

/* kernel lines */

				kh = n - l;

				if (kh >= 4 && (m & 3) == 0)
					kh = 4;
				else if (kh >= 2)
					kh = 2;

				for (off = 0; off < m; off += kw) {

/* offset in current kernel line */

					ws->x0 = x0 + (off << x_shift);

					kw = m - off;

					if (kw > 2 * MAX_KER)
						kw = MAX_KER;
					else if (kw > MAX_KER)
						kw = kw / 2;

					CALL_FUNC(u16)
				}
			}

#if IMG_TYPE < 4
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < size; i++) {
				FP_TYPE val = buff[i];

#if IMG_TYPE < 3 && defined(MLIB_USE_FTOI_CLAMPING)
				mlib_s32 ival;
#endif /* IMG_TYPE < 3 && defined(MLIB_USE_FTOI_CLAMPING) */

				SAT(dPtr[i * nchan], ival, val);

				buff[i] = sat_off;
			}

#endif /* IMG_TYPE < 4 */
		}
	}

	if (type < 4) {
		if (buff != buff_local)
			__mlib_free(buff);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#if IMG_TYPE == 0
#ifdef _MSC_VER
#pragma optimize("", on)
#endif /* _MSC_VER */
#endif /* IMG_TYPE == 0 */

#endif /* _NO_LONGLONG */
/* *********************************************************** */
