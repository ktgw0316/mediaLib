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

#ifndef _MLIB_C_IMAGEZOOMINDEX_BL_H
#define	_MLIB_C_IMAGEZOOMINDEX_BL_H

#pragma ident	"@(#)mlib_c_ImageZoomIndex_BL.h	9.2	07/10/09 SMI"

#endif /* _MLIB_C_IMAGEZOOMINDEX_BL_H */

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_c_ImageBlendTable.h>

/* *********************************************************** */

#if defined(_C_BILINEAR_U8_U8_3_)

#define	FUNC2	mlib_c_ImageZoomIndex_U8_U8_3_Bilinear_2_Linear
#define	FUNC1	mlib_c_ImageZoomIndex_U8_U8_3_Bilinear_1_Linear
#define	FUNC0	mlib_c_ImageZoomIndex_U8_U8_3_Bilinear_0_Linear
#define	FUNC_IDX	mlib_ImageColorTrue2IndexLine_U8_U8_3
#define	FUNC_MAIN	mlib_c_ImageZoomIndex_U8_U8_3_Bilinear
#define	CHL	3

#elif defined(_C_BILINEAR_U8_U8_4_)

#define	FUNC2	mlib_c_ImageZoomIndex_U8_U8_4_Bilinear_2_Linear
#define	FUNC1	mlib_c_ImageZoomIndex_U8_U8_4_Bilinear_1_Linear
#define	FUNC0	mlib_c_ImageZoomIndex_U8_U8_4_Bilinear_0_Linear
#define	FUNC_IDX	mlib_ImageColorTrue2IndexLine_U8_U8_4
#define	FUNC_MAIN	mlib_c_ImageZoomIndex_U8_U8_4_Bilinear
#define	CHL	4

#elif defined(_C_BILINEAR_S16_S16_3_)

#define	FUNC2	mlib_c_ImageZoomIndex_S16_S16_3_Bilinear_2_Linear
#define	FUNC1	mlib_c_ImageZoomIndex_S16_S16_3_Bilinear_1_Linear
#define	FUNC0	mlib_c_ImageZoomIndex_S16_S16_3_Bilinear_0_Linear
#define	FUNC_IDX	mlib_ImageColorTrue2IndexLine_S16_S16_3
#define	FUNC_MAIN	mlib_c_ImageZoomIndex_S16_S16_3_Bilinear
#define	CHL	3

#elif defined(_C_BILINEAR_S16_S16_4_)

#define	FUNC2	mlib_c_ImageZoomIndex_S16_S16_4_Bilinear_2_Linear
#define	FUNC1	mlib_c_ImageZoomIndex_S16_S16_4_Bilinear_1_Linear
#define	FUNC0	mlib_c_ImageZoomIndex_S16_S16_4_Bilinear_0_Linear
#define	FUNC_IDX	mlib_ImageColorTrue2IndexLine_S16_S16_4
#define	FUNC_MAIN	mlib_c_ImageZoomIndex_S16_S16_4_Bilinear
#define	CHL	4

#elif defined(_C_BILINEAR_U8_S16_3_)

#define	FUNC2	mlib_c_ImageZoomIndex_U8_S16_3_Bilinear_2_Linear
#define	FUNC1	mlib_c_ImageZoomIndex_U8_S16_3_Bilinear_1_Linear
#define	FUNC0	mlib_c_ImageZoomIndex_U8_S16_3_Bilinear_0_Linear
#define	FUNC_IDX	mlib_ImageColorTrue2IndexLine_S16_U8_3
#define	FUNC_MAIN	mlib_c_ImageZoomIndex_U8_S16_3_Bilinear
#define	CHL	3

#elif defined(_C_BILINEAR_U8_S16_4_)

#define	FUNC2	mlib_c_ImageZoomIndex_U8_S16_4_Bilinear_2_Linear
#define	FUNC1	mlib_c_ImageZoomIndex_U8_S16_4_Bilinear_1_Linear
#define	FUNC0	mlib_c_ImageZoomIndex_U8_S16_4_Bilinear_0_Linear
#define	FUNC_IDX	mlib_ImageColorTrue2IndexLine_S16_U8_4
#define	FUNC_MAIN	mlib_c_ImageZoomIndex_U8_S16_4_Bilinear
#define	CHL	4

#elif defined(_C_BILINEAR_S16_U8_3_)

#define	FUNC2	mlib_c_ImageZoomIndex_S16_U8_3_Bilinear_2_Linear
#define	FUNC1	mlib_c_ImageZoomIndex_S16_U8_3_Bilinear_1_Linear
#define	FUNC0	mlib_c_ImageZoomIndex_S16_U8_3_Bilinear_0_Linear
#define	FUNC_IDX	mlib_ImageColorTrue2IndexLine_U8_S16_3
#define	FUNC_MAIN	mlib_c_ImageZoomIndex_S16_U8_3_Bilinear
#define	CHL	3

#elif defined(_C_BILINEAR_S16_U8_4_)

#define	FUNC2	mlib_c_ImageZoomIndex_S16_U8_4_Bilinear_2_Linear
#define	FUNC1	mlib_c_ImageZoomIndex_S16_U8_4_Bilinear_1_Linear
#define	FUNC0	mlib_c_ImageZoomIndex_S16_U8_4_Bilinear_0_Linear
#define	FUNC_IDX	mlib_ImageColorTrue2IndexLine_U8_S16_4
#define	FUNC_MAIN	mlib_c_ImageZoomIndex_S16_U8_4_Bilinear
#define	CHL	4

#endif		   /* defined(_C_BILINEAR_U8_U8_3_) */

/* *********************************************************** */

#if defined(_PAL_U8_)

#define	PAL_TYPE	mlib_u8
#define	COEF(X)	(FTABLE(((X) >> 8) & 0xFF))

#else

#define	PAL_TYPE	mlib_s16
#define	COEF(X)	(((X) & MLIB_MASK) * MLIB_SCALE)

#endif		   /* defined(_PAL_U8_) */

/* *********************************************************** */

#if defined(_IDX_U8_)

#define	IDX_TYPE	mlib_u8

#else

#define	IDX_TYPE	mlib_s16

#endif		   /* defined(_IDX_U8_) */

/* *********************************************************** */

#define	CALC_FUNC0(N)                                           \
	r0 = buf0[CHL * i + (N)];                               \
	r1 = buf1[CHL * i + (N)];                               \
	r2 = r0 + (r1 - r0) * fdy;                              \
	tdp[CHL * i + (N)] = (PAL_TYPE) r2

/* *********************************************************** */

#define	CALC_FUNC1(N)                                           \
	a2 = *(colortable + tsp[cx + 1] * CHL + (N));           \
	a3 = *(colortable + tsp[cx] * CHL + (N));               \
	r0 = buf0[CHL * i + (N)];                               \
	r1 = a3 + (a2 - a3) * fdx;                              \
	buf1[CHL * i + (N)] = r1;                               \
	r2 = r0 + (r1 - r0) * fdy;                              \
	tdp[CHL * i + (N)] = (PAL_TYPE) r2

/* *********************************************************** */

#define	CALC_FUNC2(N)                                                \
	a0 = *(colortable + tsp[cx] * CHL + (N));                    \
	a1 = *(colortable + tsp[cx + 1] * CHL + (N));                \
	a2 = *(colortable + tsp[cx + src_stride + 1] * CHL + (N));   \
	a3 = *(colortable + tsp[cx + src_stride] * CHL + (N));       \
	r0 = a0 + (a1 - a0) * fdx;                                   \
	buf0[CHL * i + (N)] = r0;                                    \
	r1 = a3 + (a2 - a3) * fdx;                                   \
	buf1[CHL * i + (N)] = r1;                                    \
	r2 = r0 + (r1 - r0) * fdy;                                   \
	tdp[CHL * i + (N)] = (PAL_TYPE) r2

/* *********************************************************** */

static void
FUNC2(
    mlib_s32 width,
    mlib_s32 y,
    mlib_s32 x,
    mlib_s32 dx,
    mlib_s32 src_stride,
    IDX_TYPE *tsp,
    PAL_TYPE *tdp,
    mlib_d64 *colortable,
    mlib_d64 *buf0,
    mlib_d64 *buf1)
{
	mlib_d64 a0, a1, a2, a3, r0, r1, r2, fdy, fdx;
	mlib_s32 i, cx;

	fdy = COEF(y);

#ifdef __SUNPRO_C
#pragma nomemorydepend
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < width; i++) {
		fdx = COEF(x);
		cx = x >> MLIB_SHIFT;
		CALC_FUNC2(0);
		CALC_FUNC2(1);
		CALC_FUNC2(2);
#if (CHL == 4)
		CALC_FUNC2(3);
#endif /* (CHL == 4) */
		x += dx;
	}
}

/* *********************************************************** */

static void
FUNC1(
    mlib_s32 width,
    mlib_s32 y,
    mlib_s32 x,
    mlib_s32 dx,
    mlib_s32 src_stride,
    IDX_TYPE *tsp,
    PAL_TYPE *tdp,
    mlib_d64 *colortable,
    mlib_d64 *buf0,
    mlib_d64 *buf1)
{
	mlib_d64 a2, a3, r0, r1, r2, fdy, fdx;
	mlib_s32 i, cx;

	fdy = COEF(y);

	tsp += src_stride;

#ifdef __SUNPRO_C
#pragma nomemorydepend
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < width; i++) {
		fdx = COEF(x);
		cx = x >> MLIB_SHIFT;
		CALC_FUNC1(0);
		CALC_FUNC1(1);
		CALC_FUNC1(2);
#if (CHL == 4)
		CALC_FUNC1(3);
#endif /* (CHL == 4) */
		x += dx;
	}
}

/* *********************************************************** */

static void
FUNC0(
    mlib_s32 width,
    mlib_s32 y,
    PAL_TYPE *tdp,
    mlib_d64 *buf0,
    mlib_d64 *buf1)
{
	mlib_d64 r0, r1, r2, fdy;
	mlib_s32 i;

	fdy = COEF(y);

#ifdef __SUNPRO_C
#pragma nomemorydepend
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < width; i++) {
		CALC_FUNC0(0);
		CALC_FUNC0(1);
		CALC_FUNC0(2);
#if (CHL == 4)
		CALC_FUNC0(3);
#endif /* (CHL == 4) */
	}
}

/* *********************************************************** */

mlib_status
FUNC_MAIN(
    mlib_work_image *param)
{
	void *colormap = GetElemStruct(colormap);
	PAL_TYPE *dpbuf = GetElemStruct(buffer_dp);
	IDX_TYPE *tsp,
	    *dp = GetElemSubStruct(current, dp),
	    *sp = GetElemSubStruct(current, sp);
	mlib_s32 j, cx, next_step,
	    dx = GetElemStruct(DX),
	    dy = GetElemStruct(DY),
	    x = GetElemSubStruct(current, srcX) & MLIB_MASK,
	    y = GetElemSubStruct(current, srcY) & MLIB_MASK,
	    src_stride = GetElemStruct(src_stride),
	    dst_stride = GetElemStruct(dst_stride),
	    width = GetElemSubStruct(current, width),
	    height = GetElemSubStruct(current, height);
	mlib_d64 *colortable =
	    mlib_ImageGetLutDoubleData(colormap) -
	    mlib_ImageGetLutOffset(colormap) * CHL, *buf0, *buf1,
	    buffer[1024 * CHL * 2], *buff, *bufT;

	if (width > 1024) {
		buff = __mlib_malloc(sizeof (mlib_d64) * width * CHL * 2);

		if (buff == NULL)
			return (MLIB_FAILURE);
	} else
		buff = buffer;

	buf0 = buff;
	buf1 = buf0 + width * CHL;

	tsp = sp;

	next_step = 2;
	for (j = 0; j < height; j++) {

		x = GetElemSubStruct(current, srcX) & MLIB_MASK;

		cx = next_step;
		next_step = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);

		if (cx == 0) {
			FUNC0(width, y, dpbuf, buf0, buf1);
		} else {
			if (cx == 1) {
				bufT = buf0;
				buf0 = buf1;
				buf1 = bufT;
				FUNC1(width, y, x, dx, src_stride,
				    tsp, dpbuf, colortable, buf0, buf1);
			} else {
				FUNC2(width, y, x, dx, src_stride,
				    tsp, dpbuf, colortable, buf0, buf1);
			}
		}

		FUNC_IDX(dpbuf, dp, width, colormap);

		y += dy;
		dp += dst_stride;
		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
	}

	if (buff != buffer)
		__mlib_free(buff);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef FUNC2
#undef FUNC1
#undef FUNC0
#undef FUNC_IDX
#undef FUNC_MAIN
#undef CHL
#undef CALC_FUNC0
#undef CALC_FUNC1
#undef CALC_FUNC2
#undef PAL_TYPE
#undef IDX_TYPE
#undef COEF

#ifdef __cplusplus
}
#endif
