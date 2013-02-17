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

#pragma ident	"@(#)mlib_i_ImageZoomBlend_BC.c	9.2	07/11/05"

/*
 * FUNCTION
 *      mlib_ImageZoom - image scaling with edge condition
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageZoom(mlib_image       *dst,
 *                                 const mlib_image *src,
 *                                 mlib_f32         zoomx,
 *                                 mlib_f32         zoomy,
 *                                 mlib_filter      filter,
 *                                 mlib_edge        edge)
 *
 * ARGUMENTS
 *      dst       Pointer to destination image
 *      src       Pointer to source image
 *      zoomx     X zoom factor.
 *      zoomy     Y zoom factor.
 *      filter    Type of resampling filter.
 *      edge      Type of edge condition.
 *
 * DESCRIPTION
 *  The center of the source image is mapped to the center of the
 *  destination image.
 *
 *  The upper-left corner pixel of an image is located at (0.5, 0.5).
 *
 *  The resampling filter can be one of the following:
 *    MLIB_NEAREST
 *    MLIB_BILINEAR
 *    MLIB_BICUBIC
 *    MLIB_BICUBIC2
 *
 *  The edge condition can be one of the following:
 *    MLIB_EDGE_DST_NO_WRITE  (default)
 *    MLIB_EDGE_DST_FILL_ZERO
 *    MLIB_EDGE_OP_NEAREST
 *    MLIB_EDGE_SRC_EXTEND
 *    MLIB_EDGE_SRC_PADDED
 */

#ifndef	_NO_LONGLONG

#include <mlib_image.h>
#include <mlib_GeomBasicIntOp.h>

/* *********************************************************** */

#define	_MLIB_C_VER_

#include <mlib_ImageDivTables.h>
#include <mlib_ImageZoom.h>
#include <mlib_ImageFilters.h>

/* *********************************************************** */

#define	IMG_TYPE	1

#define	NO_ZOOM_FUNC

#include <mlib_i_ImageZoom_BC.h>

/* *********************************************************** */

#define	BUFF_TYPE	TTYPE

/* *********************************************************** */

#undef  SUM_Y
#define	SUM_Y(N)                                                \
	fres =                                                  \
	    k0##N * yf0 + k1##N * yf1 + k2##N * yf2 +           \
	    k3##N * yf3;                                        \
	fres >>= (FLT_SHIFT+FLT_SHIFT);                         \
	if (fres <= MLIB_U8_MIN_24)                             \
	    fres = MLIB_U8_MIN_24;                              \
	if (fres >= MLIB_U8_MAX_24)                             \
	    fres = MLIB_U8_MAX_24

/* *********************************************************** */

#undef  BLEND
#define	BLEND(dst)                                              \
{                                                               \
	mlib_s64 out, in;                                       \
	in = (a0*(fres - ((mlib_s64)dst<<24))) >> 24;           \
	DIV255(out, in);                                        \
	dst = (mlib_s32)(out) + dst;                            \
}

/* *********************************************************** */

#undef	BLEND_A
#define	BLEND_A(dst)                                            \
{                                                               \
	mlib_s64 out, in;                                       \
	dst = ((a0>>12)*((fres - ((mlib_s64)dst<<24))>>12)) >> 24; \
	DIV65025(out, in);                                      \
	dst = (mlib_s32)(out) + dst;                            \
}

/* *********************************************************** */

#define	BLEND2(dst)                                             \
	dst = dst + (mlib_s32)(((fres - ((mlib_s64)dst<<24))*MUL255(w0)/w)>>24)

#define	BLEND2_A(dst)                                           \
	dst = dst +                                             \
	(mlib_s32)(((fres - ((mlib_s64)dst<<24))*(w0>>12)/(w>>12))>>24)

#define	BLEND2z(dst)	dst = (DTYPE)((((MUL255(w0))*fres +     \
				((w1*dst)<<24))/w)>>24)

/* *********************************************************** */

#define	SUM_X(CH)                                               \
	cx = (x >> MLIB_SHIFT) * CH;                            \
	MLIB_FILTER(x, flt_table);                              \
	tt = tsp + cx;                                          \
	x += dx;                                                \
	k00 = CALCULATION(0, CH, 2 * CH, 3 * CH);               \
	tt += src_stride;                                       \
	k10 = CALCULATION(0, CH, 2 * CH, 3 * CH);               \
	tt += src_stride;                                       \
	k20 = CALCULATION(0, CH, 2 * CH, 3 * CH);               \
	tt += src_stride;                                       \
	k30 = CALCULATION(0, CH, 2 * CH, 3 * CH);               \
	SUM_Y(0)

/* *********************************************************** */

mlib_status FUNC_NAME(
    33) (
    mlib_work_image * param,
    const mlib_s16 * flt_table,
    mlib_s32 alp)
{
	VARIABLE(DTYPE);
	VARIABLE_BC(0);
	DTYPE *sl, *tsp, *tdp, *tt;
	TTYPE a0 = (param->alpha);
	mlib_s32 ch;
	BUFF_TYPE *buff;

	buff = __mlib_malloc(width * sizeof (BUFF_TYPE));

	if (buff == NULL)
		return (MLIB_FAILURE);

	sl = sp -= src_stride + 3;

	y = GetElemSubStruct(current, srcY) & MLIB_MASK;

	for (j = 0; j < height; j++) {
		for (ch = 0; ch < 3; ch++) {
			tsp = sp + ch;
			tdp = dp + ch;

			x = GetElemSubStruct(current, srcX) & MLIB_MASK;
			MLIB_FILTER(y, flt_table);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				SUM_X(3);

#if 0
				buff[i] = fres;
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				fres = buff[i];
#endif /* 0 */

				BLEND(tdp[0]);
				tdp += 3;
			}
		}

		y += dy;
		dp += dst_stride;
		sp = sl + (y >> MLIB_SHIFT) * src_stride;
	}

	__mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status FUNC_NAME(
    43) (
    mlib_work_image * param,
    const mlib_s16 * flt_table,
    mlib_s32 alp)
{
	VARIABLE(DTYPE);
	VARIABLE_BC(0);
	DTYPE *sl, *tsp, *tdp, *tt;
	TTYPE a0, dalpha = (param->alpha);
	TTYPE *buff;
	mlib_s32 ch;

	sl = sp -= src_stride + 4;

	y = GetElemSubStruct(current, srcY) & MLIB_MASK;

	buff = __mlib_malloc(width * sizeof (TTYPE));

	if (buff == NULL)
		return (MLIB_FAILURE);

	for (j = 0; j < height; j++) {
/* alpha channel */
		tsp = sp + alp;
		tdp = dp + alp;

		x = GetElemSubStruct(current, srcX) & MLIB_MASK;
		MLIB_FILTER(y, flt_table);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {
			SUM_X(4);

			buff[i] = dalpha * fres;
		}

/* other channels */
		for (ch = 0; ch < 3; ch++) {
			tsp = sp + ch;
			tdp = dp + ch;

			x = GetElemSubStruct(current, srcX) & MLIB_MASK;
			MLIB_FILTER(y, flt_table);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				SUM_X(4);

				a0 = buff[i];
				BLEND_A(tdp[0]);
				tdp += 3;
			}
		}

		y += dy;
		dp += dst_stride;
		sp = sl + (y >> MLIB_SHIFT) * src_stride;
	}

	__mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
#define	NUMERATOR_SHIFT	32
#define	NUMERATOR	((mlib_s64)1<<NUMERATOR_SHIFT)

mlib_status FUNC_NAME(
    34) (
    mlib_work_image * param,
    const mlib_s16 * flt_table,
    mlib_s32 alp)
{
	VARIABLE(DTYPE);
	VARIABLE_BC(0);
	DTYPE *sl, *tsp, *tdp, *tt;
	TTYPE *buff;
	mlib_s32 ch;
	TTYPE w0 = (TTYPE) param->alpha;
	TTYPE w1s = 255 - w0;
	TTYPE w1, w;
	mlib_blend blend = param->blend;

	sl = sp -= src_stride + 3;

	y = GetElemSubStruct(current, srcY) & MLIB_MASK;

	buff = __mlib_malloc(2 * width * sizeof (TTYPE));

	if (buff == NULL)
		return (MLIB_FAILURE);

	for (j = 0; j < height; j++) {
/* alpha channel */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {
			w1 = w1s * dp[4 * i + alp];
			w = (MUL255(w0)) + w1;

			buff[2 * i] = w1;
			buff[2 * i + 1] = w;

			mlib_s64 out;
			DIV255(out, w);
			dp[4 * i + alp] = (DTYPE) (out);
		}

/* other channels */
		for (ch = 0; ch < 3; ch++) {
			tsp = sp + ch;
			tdp = dp + ch;

			x = GetElemSubStruct(current, srcX) & MLIB_MASK;
			MLIB_FILTER(y, flt_table);

			if (blend == MLIB_BLEND_GTK_SRC_OVER2) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < width; i++) {
					SUM_X(3);

					w1 = buff[2 * i];
					w = buff[2 * i + 1];

					if (w >= 255) {
						BLEND2(tdp[0]);
					}
					tdp += 4;
				}
			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < width; i++) {
					SUM_X(3);

					w1 = buff[2 * i];
					w = buff[2 * i + 1];

					if (w < 255) {
						tdp[0] = 0;
					} else {
						BLEND2z(tdp[0]);
					}
					tdp += 4;
				}
			}
		}

		y += dy;
		dp += dst_stride;
		sp = sl + (y >> MLIB_SHIFT) * src_stride;
	}

	__mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status FUNC_NAME(
    44) (
    mlib_work_image * param,
    const mlib_s16 * flt_table,
    mlib_s32 alp)
{
	VARIABLE(DTYPE);
	VARIABLE_BC(0);
	DTYPE *sl, *tsp, *tdp, *tt;
	TTYPE *buff;
	mlib_s32 ch;
	TTYPE dalpha = (param->alpha);
	TTYPE w0, w1, w;

	sl = sp -= src_stride + 4;

	y = GetElemSubStruct(current, srcY) & MLIB_MASK;

	buff = __mlib_malloc(3 * width * sizeof (TTYPE));

	if (buff == NULL)
		return (MLIB_FAILURE);

	for (j = 0; j < height; j++) {
/* alpha channel */
		tsp = sp + alp;
		tdp = dp + alp;

		x = GetElemSubStruct(current, srcX) & MLIB_MASK;
		MLIB_FILTER(y, flt_table);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {
			SUM_X(4);

			w0 = dalpha * fres;
			w1 = (((mlib_s64)(255*255)<<24) - w0) * dp[4 * i + alp];
			w = (MUL255(w0)) + w1;

			DIV255(w0, w0);
			DIV65025(w1, w1);
			DIV65025(w, w);

			buff[3 * i] = w0;
			buff[3 * i + 1] = w1;
			buff[3 * i + 2] = w;

			dp[4 * i + alp] = (DTYPE) (w>>24);
		}

/* other channels */
		for (ch = 0; ch < 3; ch++) {
			tsp = sp + ch;
			tdp = dp + ch;

			x = GetElemSubStruct(current, srcX) & MLIB_MASK;
			MLIB_FILTER(y, flt_table);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				SUM_X(4);

				w0 = buff[3 * i];
				w1 = buff[3 * i + 1];
				w = buff[3 * i + 2];

				BLEND2_A(tdp[0]);
				tdp += 4;
			}
		}

		y += dy;
		dp += dst_stride;
		sp = sl + (y >> MLIB_SHIFT) * src_stride;
	}

	__mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#else /* _NO_LONGLONG */

#include <mlib_image.h>

/* *********************************************************** */

#define	_MLIB_C_VER_

#include <mlib_ImageDivTables.h>
#include <mlib_ImageZoom.h>
#include <mlib_ImageFilters.h>

/* *********************************************************** */

#define	IMG_TYPE	1

#define	NO_ZOOM_FUNC

#include <mlib_c_ImageZoom_BC.h>

/* *********************************************************** */

/* #undef  TTYPE */
/* #define	TTYPE	mlib_d64 */

/* *********************************************************** */

#define	BUFF_TYPE	TTYPE

/* *********************************************************** */

#undef  SUM_Y
#define	SUM_Y(N)                                                \
	fres =                                                  \
	    k0##N * yf0 + k1##N * yf1 + k2##N * yf2 +           \
	    k3##N * yf3;                                        \
	fres *= F_ONE / (1 << 24);                              \
	if (fres <= MLIB_U8_MIN)                                \
	    fres = MLIB_U8_MIN;                                 \
	if (fres >= MLIB_U8_MAX)                                \
	    fres = MLIB_U8_MAX

/* *********************************************************** */

#undef  BLEND
#define	BLEND(dst)	dst = (mlib_s32)(a0*(fres - dst)) + dst

/* *********************************************************** */

#define	BLEND2(dst)	dst = dst + (mlib_s32)((fres - dst)*(w0*rw))

#define	BLEND2z(dst)	dst = (DTYPE)((w0*fres + w1*dst)*rw)

/* *********************************************************** */

#define	SUM_X(CH)                                               \
	cx = (x >> MLIB_SHIFT) * CH;                            \
	MLIB_FILTER(x, flt_table);                              \
	tt = tsp + cx;                                          \
	x += dx;                                                \
	k00 = CALCULATION(0, CH, 2 * CH, 3 * CH);               \
	tt += src_stride;                                       \
	k10 = CALCULATION(0, CH, 2 * CH, 3 * CH);               \
	tt += src_stride;                                       \
	k20 = CALCULATION(0, CH, 2 * CH, 3 * CH);               \
	tt += src_stride;                                       \
	k30 = CALCULATION(0, CH, 2 * CH, 3 * CH);               \
	SUM_Y(0)

/* *********************************************************** */

mlib_status FUNC_NAME(
    33) (
    mlib_work_image * param,
    const FLT_TYPE * flt_table,
    mlib_s32 alp)
{
	VARIABLE(DTYPE);
	VARIABLE_BC(0);
	DTYPE *sl, *tsp, *tdp, *tt;
	TTYPE a0 = (param->alpha) * (F_ONE / 255);
	mlib_s32 ch;
	BUFF_TYPE *buff;

	buff = __mlib_malloc(width * sizeof (BUFF_TYPE));

	if (buff == NULL)
		return (MLIB_FAILURE);

	sl = sp -= src_stride + 3;

	y = GetElemSubStruct(current, srcY) & MLIB_MASK;

	for (j = 0; j < height; j++) {
		for (ch = 0; ch < 3; ch++) {
			tsp = sp + ch;
			tdp = dp + ch;

			x = GetElemSubStruct(current, srcX) & MLIB_MASK;
			MLIB_FILTER(y, flt_table);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				SUM_X(3);

#if 0
				buff[i] = fres;
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				fres = buff[i];
#endif /* 0 */

				BLEND(tdp[0]);
				tdp += 3;
			}
		}

		y += dy;
		dp += dst_stride;
		sp = sl + (y >> MLIB_SHIFT) * src_stride;
	}

	__mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status FUNC_NAME(
    43) (
    mlib_work_image * param,
    const FLT_TYPE * flt_table,
    mlib_s32 alp)
{
	VARIABLE(DTYPE);
	VARIABLE_BC(0);
	DTYPE *sl, *tsp, *tdp, *tt;
	TTYPE a0, dalpha = (param->alpha) * (F_ONE / (255 * 255));
	TTYPE *buff;
	mlib_s32 ch;

	sl = sp -= src_stride + 4;

	y = GetElemSubStruct(current, srcY) & MLIB_MASK;

	buff = __mlib_malloc(width * sizeof (TTYPE));

	if (buff == NULL)
		return (MLIB_FAILURE);

	for (j = 0; j < height; j++) {
/* alpha channel */
		tsp = sp + alp;
		tdp = dp + alp;

		x = GetElemSubStruct(current, srcX) & MLIB_MASK;
		MLIB_FILTER(y, flt_table);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {
			SUM_X(4);

			buff[i] = dalpha * fres;
		}

/* other channels */
		for (ch = 0; ch < 3; ch++) {
			tsp = sp + ch;
			tdp = dp + ch;

			x = GetElemSubStruct(current, srcX) & MLIB_MASK;
			MLIB_FILTER(y, flt_table);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				SUM_X(4);

				a0 = buff[i];
				BLEND(tdp[0]);
				tdp += 3;
			}
		}

		y += dy;
		dp += dst_stride;
		sp = sl + (y >> MLIB_SHIFT) * src_stride;
	}

	__mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status FUNC_NAME(
    34) (
    mlib_work_image * param,
    const FLT_TYPE * flt_table,
    mlib_s32 alp)
{
	VARIABLE(DTYPE);
	VARIABLE_BC(0);
	DTYPE *sl, *tsp, *tdp, *tt;
	TTYPE *buff;
	mlib_s32 ch;
	TTYPE w0 = (TTYPE) param->alpha;
	TTYPE w1s = F_ONE - w0 * (F_ONE / 255);
	TTYPE w1, w, rw;
	mlib_blend blend = param->blend;

	sl = sp -= src_stride + 3;

	y = GetElemSubStruct(current, srcY) & MLIB_MASK;

	buff = __mlib_malloc(2 * width * sizeof (TTYPE));

	if (buff == NULL)
		return (MLIB_FAILURE);

	for (j = 0; j < height; j++) {
/* alpha channel */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {
			w1 = w1s * dp[4 * i + alp];
			w = w0 + w1;
			FP_INV(rw, w);

			buff[2 * i] = w1;
			buff[2 * i + 1] = rw;

			dp[4 * i + alp] = (DTYPE) w;
		}

/* other channels */
		for (ch = 0; ch < 3; ch++) {
			tsp = sp + ch;
			tdp = dp + ch;

			x = GetElemSubStruct(current, srcX) & MLIB_MASK;
			MLIB_FILTER(y, flt_table);

			if (blend == MLIB_BLEND_GTK_SRC_OVER2) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < width; i++) {
					SUM_X(3);

					w1 = buff[2 * i];
					rw = buff[2 * i + 1];

					BLEND2(tdp[0]);
					tdp += 4;
				}
			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < width; i++) {
					SUM_X(3);

					w1 = buff[2 * i];
					rw = buff[2 * i + 1];

					BLEND2z(tdp[0]);
					tdp += 4;
				}
			}
		}

		y += dy;
		dp += dst_stride;
		sp = sl + (y >> MLIB_SHIFT) * src_stride;
	}

	__mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status FUNC_NAME(
    44) (
    mlib_work_image * param,
    const FLT_TYPE * flt_table,
    mlib_s32 alp)
{
	VARIABLE(DTYPE);
	VARIABLE_BC(0);
	DTYPE *sl, *tsp, *tdp, *tt;
	TTYPE *buff;
	mlib_s32 ch;
	TTYPE dalpha = (param->alpha) * (F_ONE / 255);
	TTYPE w0, w1, w, rw;

	sl = sp -= src_stride + 4;

	y = GetElemSubStruct(current, srcY) & MLIB_MASK;

	buff = __mlib_malloc(3 * width * sizeof (TTYPE));

	if (buff == NULL)
		return (MLIB_FAILURE);

	for (j = 0; j < height; j++) {
/* alpha channel */
		tsp = sp + alp;
		tdp = dp + alp;

		x = GetElemSubStruct(current, srcX) & MLIB_MASK;
		MLIB_FILTER(y, flt_table);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {
			SUM_X(4);

			w0 = dalpha * fres;
			w1 = (F_ONE - w0 * (F_ONE / 255)) * dp[4 * i + alp];
			w = w0 + w1;
			rw = F_ONE / w;

			buff[3 * i] = w0;
			buff[3 * i + 1] = w1;
			buff[3 * i + 2] = rw;

			dp[4 * i + alp] = (DTYPE) w;
		}

/* other channels */
		for (ch = 0; ch < 3; ch++) {
			tsp = sp + ch;
			tdp = dp + ch;

			x = GetElemSubStruct(current, srcX) & MLIB_MASK;
			MLIB_FILTER(y, flt_table);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				SUM_X(4);

				w0 = buff[3 * i];
				w1 = buff[3 * i + 1];
				rw = buff[3 * i + 2];

				BLEND2(tdp[0]);
				tdp += 4;
			}
		}

		y += dy;
		dp += dst_stride;
		sp = sl + (y >> MLIB_SHIFT) * src_stride;
	}

	__mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#endif /* _NO_LONGLONG */
