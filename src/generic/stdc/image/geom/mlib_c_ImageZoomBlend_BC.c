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

#pragma ident	"@(#)mlib_c_ImageZoomBlend_BC.c	9.3	07/10/09 SMI"

/*
 * Local functions for
 *	mlib_ImageZoomBlend()
 *	mlib_ImageZoomTranslateBlend()
 */

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

#define	BLEND2(dst)	dst = (mlib_s32)(dst + (fres - dst)*(w0*rw))

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
