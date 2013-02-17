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

#pragma ident	"@(#)mlib_i_VideoColorAVDResize_BC.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorResizeABGR - image resize
 *
 * SYNOPSIS
 *     void mlib_VideoColorResizeABGR(mlib_u32       *dst,
 *                                    const mlib_u32 *src,
 *                                    mlib_s32       dst_width,
 *                                    mlib_s32       dst_height,
 *                                    mlib_s32       dst_stride,
 *                                    mlib_s32       src_width,
 *                                    mlib_s32       src_height,
 *                                    mlib_s32       src_stride,
 *                                    mlib_filter    filter)
 *
 * ARGUMENTS
 *      dst         pointer to output image
 *      src         pointer to input image
 *      dst_stride  output image width in pixels
 *      dst_height  output image height in rows
 *      dst_stride  linebytes for output image
 *      src_width   input image width in pixels
 *      src_height  input image height in lines
 *      src_stride  linebytes for input image
 *      filter      type of interpolation filter
 *
 * DESCRIPTION
 *      Function performs the resize source image with dimensions src_width,
 *      src_height into the destination image with dimensions dst_width,
 *      dst_height using nearest-neighbor, bilinear interpolation, or bicubic
 *      interpolation. Edge conditions are handled according to "source extend"
 *      handling scheme. The interpolation filter can be one of the following:
 *        MLIB_NEAREST
 *        MLIB_BILINEAR
 *        MLIB_BICUBIC
 */

#include <mlib_video.h>
#include <mlib_VideoColorAVDResize.h>
#include <mlib_ImageDivTables.h>
#include <mlib_ImageFilters.h>

#ifdef _NO_LONGLONG
/* *********************************************************** */

#define	MLIB_SHIFT	15
#define	MLIB_PREC	(1 << MLIB_SHIFT)
#define	MLIB_PREC2	(1 << (MLIB_SHIFT - 1))
#define	MLIB_MASK	(MLIB_PREC - 1)

/* *********************************************************** */

#define	TABLE(A)	(mlib_U82F32[(A)])

/* *********************************************************** */

#define	SAT_U8	((mlib_f32)0x7F800000)
#define	SAT_U8_SH	24
#define	SAT_U8_XOR	0x80

/* *********************************************************** */

#define	VARIABLE_BC(N)                                          \
	mlib_f32 k0##N, k1##N, k2##N, k3##N;                    \
	mlib_s32 res##N

/* *********************************************************** */

#define	ISTORE(i)	tdp[4*i] = (mlib_u8)(res0)

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	MLIB_LIMITS_BC_U8(N)                                    \
	res##N =                                                \
		k0##N * yf0 + k1##N * yf1 + k2##N * yf2 +       \
		k3##N * yf3 - SAT_U8;                           \
	res##N = (res##N >> SAT_U8_SH) ^ SAT_U8_XOR

#else /* MLIB_USE_FTOI_CLAMPING */

#define	MLIB_LIMITS_BC_U8(N)                                    \
	{                                                       \
	    mlib_f32 tmp##N =                                   \
		    (k0##N * yf0 + k1##N * yf1 +                \
		    k2##N * yf2 + k3##N * yf3 - SAT_U8);        \
	    if (tmp##N >= MLIB_S32_MAX)                         \
		res##N = MLIB_U8_MAX;                           \
	    else if (tmp##N <= MLIB_S32_MIN)                    \
		res##N = MLIB_U8_MIN;                           \
	    else                                                \
		res##N =                                        \
			((mlib_s32)tmp##N >> SAT_U8_SH) ^       \
			SAT_U8_XOR;                             \
	}

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	MLIB_FILTER(X, filter)                                  \
	filterpos = ((X) >> MLIB_XY_SHIFT) & MLIB_XY_MASK;      \
	fPtr = (mlib_f32 *)((mlib_u8 *)filter + filterpos);     \
	X##f0 = fPtr[0];                                        \
	X##f1 = fPtr[1];                                        \
	X##f2 = fPtr[2];                                        \
	X##f3 = fPtr[3]

/* *********************************************************** */

#define	CALCULATION(a0, a1, a2, a3)                             \
	xf0 *                                                   \
	TABLE(tt[a0]) +                                         \
		xf1 *                                           \
	TABLE(tt[a1]) +                                         \
		xf2 *                                           \
	TABLE(tt[a2]) +                                         \
		xf3 *                                           \
	TABLE(tt[a3])

/* *********************************************************** */

#define	MLIB_XY_SHIFT	(MLIB_SHIFT - 12)
#define	MLIB_XY_MASK	(((1 << 8) - 1) << 4)

/* *********************************************************** */

void
mlib_VideoZoom_U8_4_Bicubic(
	mlib_u32 *dst,
	const mlib_u32 *src,
	mlib_s32 dx,
	mlib_s32 dy,
	mlib_s32 dw,
	mlib_s32 dh,
	mlib_s32 dst_stride,
	mlib_s32 sw,
	mlib_s32 sh,
	mlib_s32 src_stride)
{
	mlib_s32 srcX = ((dx + 1) >> 1) - MLIB_PREC2 - MLIB_PREC;
	mlib_s32 srcY = ((dy + 1) >> 1) - MLIB_PREC2 - MLIB_PREC;
	mlib_u8 *sp = (mlib_u8 *)src;
	mlib_u8 *dp = (mlib_u8 *)dst;
	mlib_f32 *fPtr, xf0, xf1, xf2, xf3, yf0, yf1, yf2, yf3;
	mlib_s32 i, j, cx, filterpos, x, y;

	VARIABLE_BC(0);
	mlib_u8 *tsp, *tdp, *tt, *sp0, *dp0 = dp;
	mlib_s32 ch, sh1 = sh - 1, sw1 = sw - 1;

	sp0 = sp;
	sw = (sw - 3) << MLIB_SHIFT;

	for (ch = 1; ch <= 4; ch++) {
		y = srcY;

		for (j = 0; j < dh; j++) {
			mlib_s32 srcy0 = y >> MLIB_SHIFT;
			mlib_s32 srcy1 = srcy0 + 1, srcy2 = srcy1 + 1, srcy3 =
				srcy2 + 1;

			srcy0 = (srcy0 < 0) ? 0 : (srcy0 > sh1) ? sh1 : srcy0;
			srcy1 = (srcy1 < 0) ? 0 : (srcy1 > sh1) ? sh1 : srcy1;
			srcy2 = (srcy2 < 0) ? 0 : (srcy2 > sh1) ? sh1 : srcy2;
			srcy3 = (srcy3 < 0) ? 0 : (srcy3 > sh1) ? sh1 : srcy3;
			srcy3 = (srcy3 - srcy2) * src_stride;
			srcy2 = (srcy2 - srcy1) * src_stride;
			srcy1 = (srcy1 - srcy0) * src_stride;

			tsp = sp + srcy0 * src_stride;
			tdp = dp;
			x = srcX;
			MLIB_FILTER(y, mlib_filters_u8f_bc);

			i = 0;

			while (x < 0 && i < dw) {
				mlib_s32 srcx0 = x >> MLIB_SHIFT;
				mlib_s32 srcx1 = srcx0 + 1, srcx2 =
					srcx1 + 1, srcx3 = srcx2 + 1;

				srcx0 = (srcx0 < 0) ? 0 : (srcx0 >
					sw1) ? sw1 : srcx0;
				srcx1 = (srcx1 < 0) ? 0 : (srcx1 >
					sw1) ? sw1 : srcx1;
				srcx2 = (srcx2 < 0) ? 0 : (srcx2 >
					sw1) ? sw1 : srcx2;
				srcx3 = (srcx3 < 0) ? 0 : (srcx3 >
					sw1) ? sw1 : srcx3;
				srcx3 = (srcx3 - srcx0) * 4;
				srcx2 = (srcx2 - srcx0) * 4;
				srcx1 = (srcx1 - srcx0) * 4;

				MLIB_FILTER(x, mlib_filters_u8f_bc);
				tt = tsp + 4 * srcx0;

				k00 = CALCULATION(0, srcx1, srcx2, srcx3);
				tt += srcy1;
				k10 = CALCULATION(0, srcx1, srcx2, srcx3);
				tt += srcy2;
				k20 = CALCULATION(0, srcx1, srcx2, srcx3);
				tt += srcy3;
				k30 = CALCULATION(0, srcx1, srcx2, srcx3);

				MLIB_LIMITS_BC_U8(0);
				ISTORE(i);

				x += dx;
				i++;
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; x < sw; i++, x += dx) {

				cx = (x >> MLIB_SHIFT) * 4;
				MLIB_FILTER(x, mlib_filters_u8f_bc);
				tt = tsp + cx;

				k00 = CALCULATION(0, 4, 8, 12);
				tt += srcy1;
				k10 = CALCULATION(0, 4, 8, 12);
				tt += srcy2;
				k20 = CALCULATION(0, 4, 8, 12);
				tt += srcy3;
				k30 = CALCULATION(0, 4, 8, 12);

				MLIB_LIMITS_BC_U8(0);
				ISTORE(i);
			}

			while (i < dw) {
				mlib_s32 srcx0 = x >> MLIB_SHIFT;
				mlib_s32 srcx1 = srcx0 + 1, srcx2 =
					srcx1 + 1, srcx3 = srcx2 + 1;

				srcx0 = (srcx0 < 0) ? 0 : (srcx0 >
					sw1) ? sw1 : srcx0;
				srcx1 = (srcx1 < 0) ? 0 : (srcx1 >
					sw1) ? sw1 : srcx1;
				srcx2 = (srcx2 < 0) ? 0 : (srcx2 >
					sw1) ? sw1 : srcx2;
				srcx3 = (srcx3 < 0) ? 0 : (srcx3 >
					sw1) ? sw1 : srcx3;
				srcx3 = (srcx3 - srcx0) * 4;
				srcx2 = (srcx2 - srcx0) * 4;
				srcx1 = (srcx1 - srcx0) * 4;

				MLIB_FILTER(x, mlib_filters_u8f_bc);
				tt = tsp + 4 * srcx0;

				k00 = CALCULATION(0, srcx1, srcx2, srcx3);
				tt += srcy1;
				k10 = CALCULATION(0, srcx1, srcx2, srcx3);
				tt += srcy2;
				k20 = CALCULATION(0, srcx1, srcx2, srcx3);
				tt += srcy3;
				k30 = CALCULATION(0, srcx1, srcx2, srcx3);

				MLIB_LIMITS_BC_U8(0);
				ISTORE(i);

				x += dx;
				i++;
			}

			y += dy;
			dp += dst_stride;
		}

		sp = sp0 + ch;
		dp = dp0 + ch;
	}
}

/* *********************************************************** */
#else
/* *********************************************************** */

#define	MLIB_SHIFT	15
#define	MLIB_PREC	(1 << MLIB_SHIFT)
#define	MLIB_PREC2	(1 << (MLIB_SHIFT - 1))
#define	MLIB_MASK	(MLIB_PREC - 1)

/* *********************************************************** */

#define	TABLE(A)	(mlib_U82F32[(A)])

/* *********************************************************** */

#define	SAT_U8	((mlib_f32)0x7F800000)
#define	SAT_U8_SH	24
#define	SAT_U8_XOR	0x80

/* *********************************************************** */

#define	VARIABLE_BC(N)                                          \
	mlib_s64 k0##N, k1##N, k2##N, k3##N;                    \
	mlib_s32 res##N

/* *********************************************************** */

#define	ISTORE(i)	tdp[4*i] = (mlib_u8)(res0)

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	MLIB_LIMITS_BC_U8(N)                                    \
	res##N =                                                \
		((k0##N * yf0 + k1##N * yf1 + k2##N * yf2 +     \
		k3##N * yf3) >> 4) - SAT_U8;                    \
	res##N = (res##N >> SAT_U8_SH) ^ SAT_U8_XOR

#else /* MLIB_USE_FTOI_CLAMPING */

#define	MLIB_LIMITS_BC_U8(N)                                    \
	{                                                       \
	    mlib_s64 tmp##N =                                   \
		    ((k0##N * yf0 + k1##N * yf1 +               \
		    k2##N * yf2 + k3##N * yf3) >> 4) - SAT_U8; \
	    if (tmp##N >= MLIB_S32_MAX)                         \
		res##N = MLIB_U8_MAX;                           \
	    else if (tmp##N <= MLIB_S32_MIN)                    \
		res##N = MLIB_U8_MIN;                           \
	    else                                                \
		res##N =                                        \
			((mlib_s32)tmp##N >> SAT_U8_SH) ^       \
			SAT_U8_XOR;                             \
	}

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	MLIB_FILTER(X, filter)                                  \
	filterpos = ((X) >> MLIB_XY_SHIFT) & MLIB_XY_MASK;      \
	fPtr = (mlib_s16 *)((mlib_u8 *)filter + (filterpos >> 1));\
	X##f0 = fPtr[0];                                        \
	X##f1 = fPtr[1];                                        \
	X##f2 = fPtr[2];                                        \
	X##f3 = fPtr[3]

/* *********************************************************** */

#define	CALCULATION(a0, a1, a2, a3)                             \
	(mlib_s64)xf0 *                                         \
	(tt[a0]) +                                         \
	(mlib_s64)xf1 *                                           \
	(tt[a1]) +                                         \
	(mlib_s64)xf2 *                                           \
	(tt[a2]) +                                         \
	(mlib_s64)xf3 *                                           \
	(tt[a3])

/* *********************************************************** */

#define	MLIB_XY_SHIFT	(MLIB_SHIFT - 12)
#define	MLIB_XY_MASK	(((1 << 8) - 1) << 4)

/* *********************************************************** */
void
mlib_VideoZoom_U8_4_Bicubic(
	mlib_u32 *dst,
	const mlib_u32 *src,
	mlib_s32 dx,
	mlib_s32 dy,
	mlib_s32 dw,
	mlib_s32 dh,
	mlib_s32 dst_stride,
	mlib_s32 sw,
	mlib_s32 sh,
	mlib_s32 src_stride)
{
	mlib_s32 srcX = ((dx + 1) >> 1) - MLIB_PREC2 - MLIB_PREC;
	mlib_s32 srcY = ((dy + 1) >> 1) - MLIB_PREC2 - MLIB_PREC;
	mlib_u8 *sp = (mlib_u8 *)src;
	mlib_u8 *dp = (mlib_u8 *)dst;
	mlib_s16 *fPtr, xf0, xf1, xf2, xf3, yf0, yf1, yf2, yf3;
	mlib_s32 i, j, cx, filterpos, x, y;
	mlib_s32  *mlib_filters_u8f_bc_INT;

	VARIABLE_BC(0);
	mlib_u8 *tsp, *tdp, *tt, *sp0, *dp0 = dp;
	mlib_s32 ch, sh1 = sh - 1, sw1 = sw - 1;

	sp0 = sp;
	sw = (sw - 3) << MLIB_SHIFT;

	for (ch = 1; ch <= 4; ch++) {
		y = srcY;

		for (j = 0; j < dh; j++) {
			mlib_s32 srcy0 = y >> MLIB_SHIFT;
			mlib_s32 srcy1 = srcy0 + 1, srcy2 = srcy1 + 1, srcy3 =
				srcy2 + 1;

			srcy0 = (srcy0 < 0) ? 0 : (srcy0 > sh1) ? sh1 : srcy0;
			srcy1 = (srcy1 < 0) ? 0 : (srcy1 > sh1) ? sh1 : srcy1;
			srcy2 = (srcy2 < 0) ? 0 : (srcy2 > sh1) ? sh1 : srcy2;
			srcy3 = (srcy3 < 0) ? 0 : (srcy3 > sh1) ? sh1 : srcy3;
			srcy3 = (srcy3 - srcy2) * src_stride;
			srcy2 = (srcy2 - srcy1) * src_stride;
			srcy1 = (srcy1 - srcy0) * src_stride;

			tsp = sp + srcy0 * src_stride;
			tdp = dp;
			x = srcX;
			MLIB_FILTER(y, mlib_filters_u8_bc);

			i = 0;

			while (x < 0 && i < dw) {
				mlib_s32 srcx0 = x >> MLIB_SHIFT;
				mlib_s32 srcx1 = srcx0 + 1, srcx2 =
					srcx1 + 1, srcx3 = srcx2 + 1;

				srcx0 = (srcx0 < 0) ? 0 : (srcx0 >
					sw1) ? sw1 : srcx0;
				srcx1 = (srcx1 < 0) ? 0 : (srcx1 >
					sw1) ? sw1 : srcx1;
				srcx2 = (srcx2 < 0) ? 0 : (srcx2 >
					sw1) ? sw1 : srcx2;
				srcx3 = (srcx3 < 0) ? 0 : (srcx3 >
					sw1) ? sw1 : srcx3;
				srcx3 = (srcx3 - srcx0) * 4;
				srcx2 = (srcx2 - srcx0) * 4;
				srcx1 = (srcx1 - srcx0) * 4;

				MLIB_FILTER(x, mlib_filters_u8_bc);
				tt = tsp + 4 * srcx0;

				k00 = CALCULATION(0, srcx1, srcx2, srcx3);
				tt += srcy1;
				k10 = CALCULATION(0, srcx1, srcx2, srcx3);
				tt += srcy2;
				k20 = CALCULATION(0, srcx1, srcx2, srcx3);
				tt += srcy3;
				k30 = CALCULATION(0, srcx1, srcx2, srcx3);

				MLIB_LIMITS_BC_U8(0);
				ISTORE(i);

				x += dx;
				i++;
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; x < sw; i++, x += dx) {

				cx = (x >> MLIB_SHIFT) * 4;
				MLIB_FILTER(x, mlib_filters_u8_bc);
				tt = tsp + cx;

				k00 = CALCULATION(0, 4, 8, 12);
				tt += srcy1;
				k10 = CALCULATION(0, 4, 8, 12);
				tt += srcy2;
				k20 = CALCULATION(0, 4, 8, 12);
				tt += srcy3;
				k30 = CALCULATION(0, 4, 8, 12);

				MLIB_LIMITS_BC_U8(0);
				ISTORE(i);
			}

			while (i < dw) {
				mlib_s32 srcx0 = x >> MLIB_SHIFT;
				mlib_s32 srcx1 = srcx0 + 1, srcx2 =
					srcx1 + 1, srcx3 = srcx2 + 1;

				srcx0 = (srcx0 < 0) ? 0 : (srcx0 >
					sw1) ? sw1 : srcx0;
				srcx1 = (srcx1 < 0) ? 0 : (srcx1 >
					sw1) ? sw1 : srcx1;
				srcx2 = (srcx2 < 0) ? 0 : (srcx2 >
					sw1) ? sw1 : srcx2;
				srcx3 = (srcx3 < 0) ? 0 : (srcx3 >
					sw1) ? sw1 : srcx3;
				srcx3 = (srcx3 - srcx0) * 4;
				srcx2 = (srcx2 - srcx0) * 4;
				srcx1 = (srcx1 - srcx0) * 4;

				MLIB_FILTER(x, mlib_filters_u8_bc);
				tt = tsp + 4 * srcx0;

				k00 = CALCULATION(0, srcx1, srcx2, srcx3);
				tt += srcy1;
				k10 = CALCULATION(0, srcx1, srcx2, srcx3);
				tt += srcy2;
				k20 = CALCULATION(0, srcx1, srcx2, srcx3);
				tt += srcy3;
				k30 = CALCULATION(0, srcx1, srcx2, srcx3);

				MLIB_LIMITS_BC_U8(0);
				ISTORE(i);

				x += dx;
				i++;
			}

			y += dy;
			dp += dst_stride;
		}

		sp = sp0 + ch;
		dp = dp0 + ch;
	}
}

/* *********************************************************** */
#endif
