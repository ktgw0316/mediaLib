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

#pragma ident	"@(#)mlib_ImageAffineTable_u16nw.c	9.3	07/11/05 SMI"

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
 *  Files mlib_ImageAffineTable_8nw.c, mlib_ImageAffineTable_16nw.c,
 *        mlib_ImageAffineTable_u16nw.c, mlib_ImageAffineTable_32nw.c,
 *        mlib_ImageAffineTable_F32nw.c, mlib_ImageAffineTable_D64nw.c,
 *        mlib_ImageAffineTable_8ext.c, mlib_ImageAffineTable_16ext.c,
 *        mlib_ImageAffineTable_u16ext.c, mlib_ImageAffineTable_32ext.c,
 *        mlib_ImageAffineTable_F32ext.c, mlib_ImageAffineTable_D64ext.c
 */

#define	IMG_TYPE	0

#if IMG_TYPE == 0
#ifdef _MSC_VER
#pragma optimize("", off)
#endif /* _MSC_VER */
#endif /* IMG_TYPE == 0 */

/* #define SRC_EXTEND */

#define	FLT_BITS	3

#define	FUNCNAME(name)	mlib_ImageAffineTableLine_F32nw_##name

#ifdef SRC_EXTEND

/* *********************************************************** */

#define	LINE_FUNC(KERN)                                           \
	void                                                      \
	mlib_ImageAffineTableLine_u16ext_##KERN(FP_TYPE * buff,   \
		FILTER_TYPE * filterX, FILTER_TYPE * filterY,     \
		const mlib_u8 **lineAddr, mlib_s32 *x_ind,        \
		mlib_affine_workspace * ws)

#else /* SRC_EXTEND */

/* *********************************************************** */

#define	LINE_FUNC(KERN)                                                 \
	void                                                            \
	mlib_ImageAffineTableLine_u16nw_##KERN(FP_TYPE * buff,          \
		FILTER_TYPE * filterX, FILTER_TYPE * filterY,           \
		const mlib_u8 **lineAddr, mlib_affine_workspace * ws)

#endif /* SRC_EXTEND */

#include <mlib_i_ImageAffineTable_func.h>

/* *********************************************************** */

mlib_status
mlib_ImageAffineTable_u16nw(
	PARAMS_NW)
{
	DECLAREVAR;
	FP_TYPE buff_local[BUFF_SIZE], *buff = buff_local;
	FP_TYPE sat_off = SAT_OFF;
	mlib_s32 sbits, x_mask;
	mlib_s32 c2_flag = 0, c3_flag = 0;

#ifndef SRC_EXTEND
#if IMG_TYPE == 4
	mlib_s32 align = (mlib_s32)lineAddr[0] | ws->srcStride;

	c2_flag = ((n & 1) | (m & 3) | (nchan & 1) | (align & 7)) == 0;
	c3_flag = (n & 1) == 0 && (m & 1) == 0 && (nchan == 3) && (type == 1);
#endif /* IMG_TYPE == 4 */
#endif /* SRC_EXTEND */

	if (type < 4) {
#if IMG_TYPE == 4
		b_step = (nchan == 4) ? 2 : nchan;
		max_xsize *= b_step;
#ifdef MLIB_USE_FTOI_CLAMPING
		sat_off = -127.5;
#else /* MLIB_USE_FTOI_CLAMPING */
		sat_off = 0.5;
#endif /* MLIB_USE_FTOI_CLAMPING */
#endif /* IMG_TYPE == 4 */

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

#ifndef SRC_EXTEND
	switch (nchan) {
	    case 1:
		    sbits = 0;
		    break;
	    case 2:
		    sbits = 1;
		    break;
	    case 3:
		    sbits = 0;
		    break;
	    case 4:
		    sbits = 2;
		    break;
	    default:
		    sbits = 0;
		    break;
	}

#else /* SRC_EXTEND */
	sbits = 0;
#endif /* SRC_EXTEND */
	x_mask = ~((1 << sbits) - 1);
	x_shift -= sbits;

	ws->x_shift = x_shift;
	ws->x_mask = x_mask;
	ws->xf_shift = xf_shift;
	ws->xf_mask = xf_mask;
	ws->yf_shift = yf_shift;
	ws->yf_mask = yf_mask;

	for (j = yStart; j <= yFinish; j++) {
		old_size = size;
		CLIP(CHAN1);

		if (type < 4) {
#if IMG_TYPE == 4
/*
 * u8 via F32 image
 */

			if (c2_flag || c3_flag)
				b_step = (nchan == 4) ? 2 : nchan;
			else
				b_step = 1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = b_step * old_size; i < b_step * size; i++) {
				buff[i] = sat_off;
			}

#else /* IMG_TYPE == 4 */
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

#endif /* IMG_TYPE == 4 */
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

			if (c2_flag && (k & 1))
				continue;

			if (c3_flag && k)
				continue;

			ws->k = k;

			if (type >= 4) {
				buff = (void *)(dstPixelPtr + k);
			}

			for (l = 0; l < n; l += kh) {

/* kernel lines */

				kh = n - l;

				if (kh >= 4 && (m & 3) == 0 &&
					!(c2_flag | c3_flag))
					kh = 4;
				else if (kh >= 2)
					kh = 2;

				for (off = 0; off < m; off += kw) {

/* offset in current kernel line */

					ws->x0 = x0 + (off << (x_shift +
						sbits));

					kw = m - off;

					if (kw > 2 * MAX_KER)
						kw = MAX_KER;
					else if (kw > MAX_KER)
						kw = kw / 2;

#ifndef SRC_EXTEND
#if IMG_TYPE == 4

					if (c3_flag) {
						kw = 2;
						FUNCNAME(c3_2_2)
							(buff, filterX + off,
							filterY + l,
							lineAddr + l, ws);
						continue;
					}

					if (c2_flag) {
						if (nchan == 2) {
							FUNCNAME(c2_2_4)
								(buff,
								filterX + off,
								filterY + l,
								lineAddr + l,
								ws);
						} else {
							FUNCNAME(c4_2_4)
								(buff,
								filterX + off,
								filterY + l,
								lineAddr + l,
								ws);
						}
					} else
#endif /* IMG_TYPE == 4 */
#endif /* SRC_EXTEND */

						CALL_FUNC(u16);
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

#if IMG_TYPE == 4

			if (type == 1) {
				mlib_u8 *dp =
					(mlib_u8 *)dstData + nchan * xLeft + k;

				if (c3_flag) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (i = 0; i < 3 * size; i++) {
						FP_TYPE val = (FP_TYPE) buff[i];

#ifdef MLIB_USE_FTOI_CLAMPING
						mlib_s32 ival;
#endif /* MLIB_USE_FTOI_CLAMPING */
						SAT8(dp[i], ival, val);

						buff[i] = sat_off;
					}
				} else if (c2_flag) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (i = 0; i < size; i++) {
						FP_TYPE val0 =
							(FP_TYPE) buff[2 * i];
						FP_TYPE val1 =
							(FP_TYPE) buff[2 * i +
							1];
#ifdef MLIB_USE_FTOI_CLAMPING
						mlib_s32 ival0, ival1;
#endif /* MLIB_USE_FTOI_CLAMPING */
						SAT8(dp[i * nchan], ival0,
							val0);
						SAT8(dp[i * nchan + 1], ival1,
							val1);

						buff[2 * i] = sat_off;
						buff[2 * i + 1] = sat_off;
					}
				} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (i = 0; i < size; i++) {
						FP_TYPE val = (FP_TYPE) buff[i];

#ifdef MLIB_USE_FTOI_CLAMPING
						mlib_s32 ival;
#endif /* MLIB_USE_FTOI_CLAMPING */
						SAT8(dp[i * nchan], ival, val);

						buff[i] = sat_off;
					}
				}
			}
#endif /* IMG_TYPE == 4 */
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
 *  Files mlib_ImageAffineTable_8nw.c, mlib_ImageAffineTable_16nw.c,
 *        mlib_ImageAffineTable_u16nw.c, mlib_ImageAffineTable_32nw.c,
 *        mlib_ImageAffineTable_F32nw.c, mlib_ImageAffineTable_D64nw.c,
 *        mlib_ImageAffineTable_8ext.c, mlib_ImageAffineTable_16ext.c,
 *        mlib_ImageAffineTable_u16ext.c, mlib_ImageAffineTable_32ext.c,
 *        mlib_ImageAffineTable_F32ext.c, mlib_ImageAffineTable_D64ext.c
 */

#define	IMG_TYPE	0

#if IMG_TYPE == 0
#ifdef _MSC_VER
#pragma optimize("", off)
#endif /* _MSC_VER */
#endif /* IMG_TYPE == 0 */

/* #define SRC_EXTEND */

#define	FLT_BITS	3

#define	FUNCNAME(name)	mlib_ImageAffineTableLine_F32nw_##name

#ifdef SRC_EXTEND

/* *********************************************************** */

#define	LINE_FUNC(KERN)                                           \
	void                                                      \
	mlib_ImageAffineTableLine_u16ext_##KERN(FP_TYPE * buff,   \
		FILTER_TYPE * filterX, FILTER_TYPE * filterY,     \
		const mlib_u8 **lineAddr, mlib_s32 *x_ind,        \
		mlib_affine_workspace * ws)

#else /* SRC_EXTEND */

/* *********************************************************** */

#define	LINE_FUNC(KERN)                                                 \
	void                                                            \
	mlib_ImageAffineTableLine_u16nw_##KERN(FP_TYPE * buff,          \
		FILTER_TYPE * filterX, FILTER_TYPE * filterY,           \
		const mlib_u8 **lineAddr, mlib_affine_workspace * ws)

#endif /* SRC_EXTEND */

#include <mlib_ImageAffineTable_func.h>

/* *********************************************************** */

mlib_status
mlib_ImageAffineTable_u16nw(
	PARAMS_NW)
{
	DECLAREVAR;
	FP_TYPE buff_local[BUFF_SIZE], *buff = buff_local;
	FP_TYPE sat_off = SAT_OFF;
	mlib_s32 sbits, x_mask;
	mlib_s32 c2_flag = 0, c3_flag = 0;

#ifndef SRC_EXTEND
#if IMG_TYPE == 4
	mlib_s32 align = (mlib_s32)lineAddr[0] | ws->srcStride;

	c2_flag = ((n & 1) | (m & 3) | (nchan & 1) | (align & 7)) == 0;
	c3_flag = (n & 1) == 0 && (m & 1) == 0 && (nchan == 3) && (type == 1);
#endif /* IMG_TYPE == 4 */
#endif /* SRC_EXTEND */

	if (type < 4) {
#if IMG_TYPE == 4
		b_step = (nchan == 4) ? 2 : nchan;
		max_xsize *= b_step;
#ifdef MLIB_USE_FTOI_CLAMPING
		sat_off = -127.5;
#else /* MLIB_USE_FTOI_CLAMPING */
		sat_off = 0.5;
#endif /* MLIB_USE_FTOI_CLAMPING */
#endif /* IMG_TYPE == 4 */

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

#ifndef SRC_EXTEND
	switch (nchan) {
	    case 1:
		    sbits = 0;
		    break;
	    case 2:
		    sbits = 1;
		    break;
	    case 3:
		    sbits = 0;
		    break;
	    case 4:
		    sbits = 2;
		    break;
	    default:
		    sbits = 0;
		    break;
	}

#else /* SRC_EXTEND */
	sbits = 0;
#endif /* SRC_EXTEND */
	x_mask = ~((1 << sbits) - 1);
	x_shift -= sbits;

	ws->x_shift = x_shift;
	ws->x_mask = x_mask;
	ws->xf_shift = xf_shift;
	ws->xf_mask = xf_mask;
	ws->yf_shift = yf_shift;
	ws->yf_mask = yf_mask;

	for (j = yStart; j <= yFinish; j++) {
		old_size = size;
		CLIP(CHAN1);

		if (type < 4) {
#if IMG_TYPE == 4
/*
 * u8 via F32 image
 */

			if (c2_flag || c3_flag)
				b_step = (nchan == 4) ? 2 : nchan;
			else
				b_step = 1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = b_step * old_size; i < b_step * size; i++) {
				buff[i] = sat_off;
			}

#else /* IMG_TYPE == 4 */
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

#endif /* IMG_TYPE == 4 */
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

			if (c2_flag && (k & 1))
				continue;

			if (c3_flag && k)
				continue;

			ws->k = k;

			if (type >= 4) {
				buff = (void *)(dstPixelPtr + k);
			}

			for (l = 0; l < n; l += kh) {

/* kernel lines */

				kh = n - l;

				if (kh >= 4 && (m & 3) == 0 &&
					!(c2_flag | c3_flag))
					kh = 4;
				else if (kh >= 2)
					kh = 2;

				for (off = 0; off < m; off += kw) {

/* offset in current kernel line */

					ws->x0 = x0 + (off << (x_shift +
						sbits));

					kw = m - off;

					if (kw > 2 * MAX_KER)
						kw = MAX_KER;
					else if (kw > MAX_KER)
						kw = kw / 2;

#ifndef SRC_EXTEND
#if IMG_TYPE == 4

					if (c3_flag) {
						kw = 2;
						FUNCNAME(c3_2_2)
							(buff, filterX + off,
							filterY + l,
							lineAddr + l, ws);
						continue;
					}

					if (c2_flag) {
						if (nchan == 2) {
							FUNCNAME(c2_2_4)
								(buff,
								filterX + off,
								filterY + l,
								lineAddr + l,
								ws);
						} else {
							FUNCNAME(c4_2_4)
								(buff,
								filterX + off,
								filterY + l,
								lineAddr + l,
								ws);
						}
					} else
#endif /* IMG_TYPE == 4 */
#endif /* SRC_EXTEND */

						CALL_FUNC(u16);
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

#if IMG_TYPE == 4

			if (type == 1) {
				mlib_u8 *dp =
					(mlib_u8 *)dstData + nchan * xLeft + k;

				if (c3_flag) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (i = 0; i < 3 * size; i++) {
						FP_TYPE val = (FP_TYPE) buff[i];

#ifdef MLIB_USE_FTOI_CLAMPING
						mlib_s32 ival;
#endif /* MLIB_USE_FTOI_CLAMPING */
						SAT8(dp[i], ival, val);

						buff[i] = sat_off;
					}
				} else if (c2_flag) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (i = 0; i < size; i++) {
						FP_TYPE val0 =
							(FP_TYPE) buff[2 * i];
						FP_TYPE val1 =
							(FP_TYPE) buff[2 * i +
							1];
#ifdef MLIB_USE_FTOI_CLAMPING
						mlib_s32 ival0, ival1;
#endif /* MLIB_USE_FTOI_CLAMPING */
						SAT8(dp[i * nchan], ival0,
							val0);
						SAT8(dp[i * nchan + 1], ival1,
							val1);

						buff[2 * i] = sat_off;
						buff[2 * i + 1] = sat_off;
					}
				} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (i = 0; i < size; i++) {
						FP_TYPE val = (FP_TYPE) buff[i];

#ifdef MLIB_USE_FTOI_CLAMPING
						mlib_s32 ival;
#endif /* MLIB_USE_FTOI_CLAMPING */
						SAT8(dp[i * nchan], ival, val);

						buff[i] = sat_off;
					}
				}
			}
#endif /* IMG_TYPE == 4 */
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
