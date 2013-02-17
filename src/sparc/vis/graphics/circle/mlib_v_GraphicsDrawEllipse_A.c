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

#pragma ident	"@(#)mlib_v_GraphicsDrawEllipse_A.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_GraphicsDrawEllipse_A_[8 | 32] - draw ellipse
 *
 * SYNOPSIS
 *      mlib_status mlib_GraphicsDrawEllipse_A_8 (mlib_image *buffer,
 *                                                mlib_s16   x,
 *                                                mlib_s16   y,
 *                                                mlib_s32   a,
 *                                                mlib_s32   b,
 *                                                mlib_f32   t,
 *                                                mlib_s32   c);
 *      mlib_status mlib_GraphicsDrawEllipse_A_32(mlib_image *buffer,
 *                                                mlib_s16   x,
 *                                                mlib_s16   y,
 *                                                mlib_s32   a,
 *                                                mlib_s32   b,
 *                                                mlib_f32   t,
 *                                                mlib_s32   c);
 *
 * ARGUMENTS
 *      buffer  pointer to the image which the function is drawing into
 *      x, y    coordinates of the center
 *      a       major semiaxis of the ellipse
 *      b       minor semiaxis of the ellipse
 *      t       angle of major semiaxis in radians
 *      c       color used in the drawing
 * DESCRIPTION
 *      Function draws an ellipse in antialiasing mode at (x, y),
 *      major semiaxis a, minor semiaxis b, and the angle of the major
 *      semiaxis is t clockwise from X axis.
 */

#include <mlib_graphics.h>
#include <vis_proto.h>
#include <mlib_SysMath.h>
#include <mlib_v_GraphicsTabAlias.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_GraphicsDrawEllipse_A_32 = __mlib_GraphicsDrawEllipse_A_32
#pragma weak mlib_GraphicsDrawEllipse_A_8 = __mlib_GraphicsDrawEllipse_A_8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_GraphicsDrawEllipse_A_32) mlib_GraphicsDrawEllipse_A_32
    __attribute__((weak, alias("__mlib_GraphicsDrawEllipse_A_32")));
__typeof__(__mlib_GraphicsDrawEllipse_A_8) mlib_GraphicsDrawEllipse_A_8
    __attribute__((weak, alias("__mlib_GraphicsDrawEllipse_A_8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	max(a, b)	((a) < (b) ? (b) : (a))
#define	min(a, b)	((a) > (b) ? (b) : (a))

/* *********************************************************** */

#define	SQRT2_256	181.019335984f

/* *********************************************************** */

#define	INSIDEX(X)	((X) < width && (X) >= 0)
#define	INSIDEY(Y)	((Y) < height && (Y) >= 0)
#define	INSIDE(X, Y)	(INSIDEX(X) && INSIDEY(Y))

/* *********************************************************** */

#define	CALC_CCOEF(coef)                                        \
	vis_fmul8x16(cf, coef);                                 \
	coef = vis_fpsub16(d_one, coef)

/* *********************************************************** */

#define	LDCOEFD(ind)	((mlib_d64 *)mlib_v_TabAlias)[ind]

/* *********************************************************** */

#define	MAKE_3_COEF                                             \
	coef0 = LDCOEFD(ind0);                                  \
	coef1 = LDCOEFD(ind1);                                  \
	coef2 = LDCOEFD(ind2);                                  \
	ccoef0 = CALC_CCOEF(coef0);                             \
	ccoef1 = CALC_CCOEF(coef1);                             \
	ccoef2 = CALC_CCOEF(coef2)

/* *********************************************************** */

#define	MAKE_3_COEF_REVERSE                                     \
	coef0 = LDCOEFD(ind0);                                  \
	coef1 = LDCOEFD(ind2);                                  \
	coef2 = LDCOEFD(ind1);                                  \
	ccoef0 = CALC_CCOEF(coef0);                             \
	ccoef1 = CALC_CCOEF(coef1);                             \
	ccoef2 = CALC_CCOEF(coef2)

/* *********************************************************** */

#define	PCALC8(val, ind)                                        \
	vis_fpack16(vis_fpadd16(ccoef##ind,                     \
	    vis_fmul8x16(vis_read_lo(val), coef##ind)))

/* *********************************************************** */

#define	PPROC8(ptr, ind)                                        \
	{                                                       \
	    mlib_d64 v = vis_ld_u8((ptr));                      \
	                                                        \
	    ((mlib_f32 *)&v)[1] = PCALC8(v, ind);               \
	    vis_st_u8(v, (ptr));                                \
	}

/* *********************************************************** */

#define	PPROC8_6(p00, p01, p02, p10, p11, p12, i0, i1, i2)          \
	{                                                           \
	    mlib_d64 v00, v01, v02, v10, v11, v12;                  \
	                                                            \
	    v00 = vis_ld_u8(p00);                                   \
	    v01 = vis_ld_u8(p01);                                   \
	    v02 = vis_ld_u8(p02);                                   \
	    v10 = vis_ld_u8(p10);                                   \
	    v11 = vis_ld_u8(p11);                                   \
	    v12 = vis_ld_u8(p12);                                   \
	    vis_st_u8(vis_write_lo(v00, PCALC8(v00, i0)), (p00));   \
	    vis_st_u8(vis_write_lo(v01, PCALC8(v01, i1)), (p01));   \
	    vis_st_u8(vis_write_lo(v02, PCALC8(v02, i2)), (p02));   \
	    vis_st_u8(vis_write_lo(v10, PCALC8(v10, i0)), (p10));   \
	    vis_st_u8(vis_write_lo(v11, PCALC8(v11, i2)), (p11));   \
	    vis_st_u8(vis_write_lo(v12, PCALC8(v12, i1)), (p12));   \
	}

/* *********************************************************** */

#define	PCALC32(val, ind)                                         \
	vis_fpack16(vis_fpadd16(ccoef##ind, vis_fmul8x16((val),   \
	    coef##ind)))

/* *********************************************************** */

#define	PPROC32(ptr, cind)                                      \
	{                                                       \
	    mlib_f32 p = *(mlib_f32 *)(ptr);                    \
	                                                        \
	    p = PCALC32(p, cind);                               \
	    *(mlib_f32 *)(ptr) = p;                             \
	}

/* *********************************************************** */

#define	PPROC32_6(p00, p01, p02, p10, p11, p12, i0, i1, i2)     \
	{                                                       \
	    mlib_f32 v00, v01, v02, v10, v11, v12;              \
	                                                        \
	    v00 = *(mlib_f32 *)(p00);                           \
	    v01 = *(mlib_f32 *)(p01);                           \
	    v02 = *(mlib_f32 *)(p02);                           \
	    v10 = *(mlib_f32 *)(p10);                           \
	    v11 = *(mlib_f32 *)(p11);                           \
	    v12 = *(mlib_f32 *)(p12);                           \
	    *(mlib_f32 *)(p00) = PCALC32(v00, i0);              \
	    *(mlib_f32 *)(p01) = PCALC32(v01, i1);              \
	    *(mlib_f32 *)(p02) = PCALC32(v02, i2);              \
	    *(mlib_f32 *)(p10) = PCALC32(v10, i0);              \
	    *(mlib_f32 *)(p11) = PCALC32(v11, i2);              \
	    *(mlib_f32 *)(p12) = PCALC32(v12, i1);              \
	}

/* *********************************************************** */

#define	SIMPLE_CALCULATE_DISTANCE_X                             \
	fcx = cx;                                               \
	fcy = cy;                                               \
	ix2 = ab * mlib_sqrtf(b2 - fcy * fcy);                  \
	l = fcx - ix2;                                          \
	zz = (ix2 - sc) * y0;                                   \
	find0 = l * zz;                                         \
	ind0 = find0;                                           \
	if (ind0 < 0)                                           \
	    ind0 = -ind0;                                       \
	ind1 = find0 + zz;                                      \
	ind2 = zz - find0

/* *********************************************************** */

#define	SIMPLE_CALCULATE_DISTANCE_Y                             \
	fcx = cx;                                               \
	fcy = cy;                                               \
	iy2 = ba * mlib_sqrtf(a2 - fcx * fcx);                  \
	l = fcy - iy2;                                          \
	zz = (iy2 + sc) * x0;                                   \
	find0 = l * zz;                                         \
	ind0 = find0;                                           \
	if (ind0 < 0)                                           \
	    ind0 = -ind0;                                       \
	ind1 = find0 + zz;                                      \
	ind2 = zz - find0

/* *********************************************************** */

#define	SIMPLE_CALCULATE_DISTANCE_MIDDLE                        \
	fcx = cx;                                               \
	fcy = cy;                                               \
	ix2 = ab * mlib_sqrtf(b2 - fcy * fcy);                  \
	l = fcx - ix2;                                          \
	zz = (ix2 - sc) * y0;                                   \
	find0 = l * zz;                                         \
	ind0 = find0 + zz

/* *********************************************************** */

#define	SIMPLE_PUT_NOCLIP_X(bits)                                         \
	PPROC##bits##_6(line0 + cx, line0 + cx + 1,                       \
	    line0 + cx - 1, line0 - cx, line0 - cx + 1, line0 - cx - 1,   \
	    0, 1, 2);                                                     \
	if (cy) {                                                         \
	    PPROC##bits##_6(line1 + cx, line1 + cx + 1,                   \
		line1 + cx - 1, line1 - cx, line1 - cx + 1,               \
		line1 - cx - 1, 0, 1, 2);                                 \
	}

/* *********************************************************** */

#define	SIMPLE_PUT_NOCLIP_Y(bits)                                       \
	PPROC##bits##_6(line0 + cx, line0 + cx - stride,                \
	    line0 + cx + stride, line1 + cx, line1 + cx - stride,       \
	    line1 + cx + stride, 0, 1, 2);                              \
	if (cx) {                                                       \
	    PPROC##bits##_6(line0 - cx, line0 - cx - stride,            \
		line0 - cx + stride, line1 - cx, line1 - cx - stride,   \
		line1 - cx + stride, 0, 1, 2);                          \
	}

/* *********************************************************** */

#define	SIMPLE_PUT_CLIP_X(bits)                                 \
	if (INSIDEY(y - cy)) {                                  \
	    if (INSIDEX(x + cx))                                \
		PPROC##bits(line0 + cx, 0);                     \
	    if (INSIDEX(x + cx - 1))                            \
		PPROC##bits(line0 + cx - 1, 2);                 \
	    if (INSIDEX(x + cx + 1))                            \
		PPROC##bits(line0 + cx + 1, 1);                 \
	    if (INSIDEX(x - cx))                                \
		PPROC##bits(line0 - cx, 0);                     \
	    if (INSIDEX(x - cx - 1))                            \
		PPROC##bits(line0 - cx - 1, 1);                 \
	    if (INSIDEX(x - cx + 1))                            \
		PPROC##bits(line0 - cx + 1, 2);                 \
	}                                                       \
	if (INSIDEY(y + cy) && cy) {                            \
	    if (INSIDEX(x + cx))                                \
		PPROC##bits(line1 + cx, 0);                     \
	    if (INSIDEX(x + cx - 1))                            \
		PPROC##bits(line1 + cx - 1, 2);                 \
	    if (INSIDEX(x + cx + 1))                            \
		PPROC##bits(line1 + cx + 1, 1);                 \
	    if (INSIDEX(x - cx))                                \
		PPROC##bits(line1 - cx, 0);                     \
	    if (INSIDEX(x - cx - 1))                            \
		PPROC##bits(line1 - cx - 1, 1);                 \
	    if (INSIDEX(x - cx + 1))                            \
		PPROC##bits(line1 - cx + 1, 2);                 \
	}

/* *********************************************************** */

#define	SIMPLE_PUT_CLIP_Y(bits)                                 \
	if (INSIDEX(x + cx)) {                                  \
	    if (INSIDEY(y - cy))                                \
		PPROC##bits(line0 + cx, 0);                     \
	    if (INSIDEY(y - cy + 1))                            \
		PPROC##bits(line0 + cx + stride, 2);            \
	    if (INSIDEY(y - cy - 1))                            \
		PPROC##bits(line0 + cx - stride, 1);            \
	    if (INSIDEY(y + cy))                                \
		PPROC##bits(line1 + cx, 0);                     \
	    if (INSIDEY(y + cy + 1))                            \
		PPROC##bits(line1 + cx + stride, 1);            \
	    if (INSIDEY(y + cy - 1))                            \
		PPROC##bits(line1 + cx - stride, 2);            \
	}                                                       \
	if (INSIDEX(x - cx) && cx) {                            \
	    if (INSIDEY(y - cy))                                \
		PPROC##bits(line0 - cx, 0);                     \
	    if (INSIDEY(y - cy + 1))                            \
		PPROC##bits(line0 - cx + stride, 2);            \
	    if (INSIDEY(y - cy - 1))                            \
		PPROC##bits(line0 - cx - stride, 1);            \
	    if (INSIDEY(y + cy))                                \
		PPROC##bits(line1 - cx, 0);                     \
	    if (INSIDEY(y + cy + 1))                            \
		PPROC##bits(line1 - cx + stride, 1);            \
	    if (INSIDEY(y + cy - 1))                            \
		PPROC##bits(line1 - cx - stride, 2);            \
	}

/* *********************************************************** */

#define	SIMPLE_PUT_NOCLIP_MIDDLE(bits)                          \
	PPROC##bits(line0 + cx + 1, 0);                         \
	PPROC##bits(line0 - cx - 1, 0);                         \
	PPROC##bits(line1 + cx + 1, 0);                         \
	PPROC##bits(line1 - cx - 1, 0)

/* *********************************************************** */

#define	SIMPLE_PUT_CLIP_MIDDLE(bits)                            \
	if (INSIDEX(x + cx + 1)) {                              \
	    if (INSIDEY(y - cy))                                \
		PPROC##bits(line0 + cx + 1, 0);                 \
	    if (INSIDEY(y + cy))                                \
		PPROC##bits(line1 + cx + 1, 0);                 \
	}                                                       \
	if (INSIDEX(x - cx - 1)) {                              \
	    if (INSIDEY(y - cy))                                \
		PPROC##bits(line0 - cx - 1, 0);                 \
	    if (INSIDEY(y + cy))                                \
		PPROC##bits(line1 - cx - 1, 0);                 \
	}

/* *********************************************************** */

#define	CHECKINDEX(ind)                                         \
	if (ind < 0)                                            \
	    ind = -ind;                                         \
	if (ind > 384)                                          \
	    ind = 384

/* *********************************************************** */

#define	ROTATED_CALCULATE_DISTANCE                              \
	fcx1 = cx;                                              \
	fcy1 = cy;                                              \
	fcx = fcx1 * cosfi + fcy1 * sinfi;                      \
	fcy = fcy1 * cosfi - fcx1 * sinfi;                      \
	if (zone) {                                             \
	                                                        \
/*                                                              \
 * vertical semiaxis                                            \
 */                                                             \
	    mlib_f32 square = a2 - fcx * fcx;                   \
	                                                        \
	    if (square > 0.0)                                   \
		iy2 = ba * mlib_sqrtf(square);                  \
	    else                                                \
		iy2 = 0.0;                                      \
	    if (fcy > 0.0f) {                                   \
		l = fcy - iy2;                                  \
		zz = (iy2 + sc) * x0;                           \
		find0 = l * zz;                                 \
		ind0 = (mlib_s32)find0;                         \
		CHECKINDEX(ind0);                               \
		ind1 = find0 + zz;                              \
		CHECKINDEX(ind1);                               \
		ind2 = zz - find0;                              \
		CHECKINDEX(ind2);                               \
	    } else {                                            \
		l = fcy + iy2;                                  \
		zz = (-iy2 - sc) * x0;                          \
		find0 = l * zz;                                 \
		ind0 = (mlib_s32)find0;                         \
		CHECKINDEX(ind0);                               \
		ind1 = find0 - zz;                              \
		CHECKINDEX(ind1);                               \
		ind2 = -find0 - zz;                             \
		CHECKINDEX(ind2);                               \
	    }                                                   \
	} else {                                                \
	                                                        \
/*                                                              \
 * horizontal semiaxis                                          \
 */                                                             \
	    mlib_f32 square = b2 - fcx * fcx;                   \
	                                                        \
	    if (square > 0.0)                                   \
		ix2 = ab * mlib_sqrtf(square);                  \
	    else                                                \
		ix2 = 0.0;                                      \
	    if (fcx > 0.0f) {                                   \
		l = fcx - ix2;                                  \
		zz = (ix2 - sc) * y0;                           \
		find0 = l * zz;                                 \
		ind0 = (mlib_s32)find0;                         \
		CHECKINDEX(ind0);                               \
		ind1 = find0 + zz;                              \
		CHECKINDEX(ind1);                               \
		ind2 = zz - find0;                              \
		CHECKINDEX(ind2);                               \
	    } else {                                            \
		l = fcx + ix2;                                  \
		zz = (-ix2 + sc) * y0;                          \
		find0 = l * zz;                                 \
		ind0 = (mlib_s32)find0;                         \
		CHECKINDEX(ind0);                               \
		ind1 = find0 - zz;                              \
		CHECKINDEX(ind1);                               \
		ind2 = -find0 - zz;                             \
		CHECKINDEX(ind2);                               \
	    }                                                   \
	}

/* *********************************************************** */

#define	ROTATED_PUT_NOCLIP_X(bits)                              \
	PPROC##bits##_6(line0, line0 - 1, line0 + 1, line1,     \
	    line1 - 1, line1 + 1, 0, 1, 2)

/* *********************************************************** */

#define	ROTATED_PUT_NOCLIP_Y(bits)                               \
	PPROC##bits##_6(line0, line0 - stride, line0 + stride,   \
	    line1, line1 - stride, line1 + stride, 0, 1, 2)

/* *********************************************************** */

#define	ROTATED_PUT_NOCLIP_CORNER(bits, delta)                    \
	PPROC##bits##_6(line0, line0 - stride, line0 + (delta),   \
	    line1, line1 + stride, line1 - (delta), 0, 1, 2)

/* *********************************************************** */

#define	ROTATED_PUT_CLIP_X(bits)                                \
	if (INSIDEY(y - cy)) {                                  \
	    if (INSIDEX(x + cx))                                \
		PPROC##bits(line0, 0);                          \
	    if (INSIDEX(x + cx - 1))                            \
		PPROC##bits(line0 - 1, 1);                      \
	    if (INSIDEX(x + cx + 1))                            \
		PPROC##bits(line0 + 1, 2);                      \
	}                                                       \
	if (INSIDEY(y + cy)) {                                  \
	    if (INSIDEX(x - cx))                                \
		PPROC##bits(line1, 0);                          \
	    if (INSIDEX(x - cx - 1))                            \
		PPROC##bits(line1 - 1, 2);                      \
	    if (INSIDEX(x - cx + 1))                            \
		PPROC##bits(line1 + 1, 1);                      \
	}

/* *********************************************************** */

#define	ROTATED_PUT_CLIP_Y(bits)                                \
	if (INSIDEX(x + cx)) {                                  \
	    if (INSIDEY(y - cy))                                \
		PPROC##bits(line0, 0);                          \
	    if (INSIDEY(y - cy + 1))                            \
		PPROC##bits(line0 + stride, 2);                 \
	    if (INSIDEY(y - cy - 1))                            \
		PPROC##bits(line0 - stride, 1);                 \
	}                                                       \
	if (INSIDEX(x - cx)) {                                  \
	    if (INSIDEY(y + cy))                                \
		PPROC##bits(line1, 0);                          \
	    if (INSIDEY(y + cy + 1))                            \
		PPROC##bits(line1 + stride, 1);                 \
	    if (INSIDEY(y + cy - 1))                            \
		PPROC##bits(line1 - stride, 2);                 \
	}

/* *********************************************************** */

#define	ROTATED_PUT_CLIP_CORNER(bits, delta)                    \
	if (INSIDEX(x + cx)) {                                  \
	    if (INSIDEY(y - cy))                                \
		PPROC##bits(line0, 0);                          \
	    if (INSIDEY(y - cy - 1))                            \
		PPROC##bits(line0 - stride, 1);                 \
	}                                                       \
	if (INSIDE(x + cx + (delta), y - cy))                   \
	    PPROC##bits(line0 + (delta), 2);                    \
	if (INSIDEX(x - cx)) {                                  \
	    if (INSIDEY(y + cy))                                \
		PPROC##bits(line1, 0);                          \
	    if (INSIDEY(y + cy + 1))                            \
		PPROC##bits(line1 + stride, 1);                 \
	}                                                       \
	if (INSIDE(x - cx - (delta), y + cy))                   \
	    PPROC##bits(line1 - (delta), 2)

/* *********************************************************** */

#define	PHASE_END	}

/* *********************************************************** */

#define	ROTATED_ELLIPSE_PHASE1_START                                      \
	                                                                  \
/*                                                                        \
 * zone 1: x = x + 1 / 2, y = y + 1                                       \
 */                                                                       \
	fcx = cx;                                                         \
	fcy = cy;                                                         \
	_2fcx = fcx * 2.0f;                                               \
	_2fcy = fcy * 2.0f;                                               \
	fcx1 = fcx + 1.0f;                                                \
	fcy1 = fcy + 1.0f;                                                \
	fcx21 = fcx1 * 2.0f;                                              \
	fcy21 = fcy1 * 2.0f;                                              \
	_2fcx1 = _2fcx + 1.0f;                                            \
	_2fcy3 = _2fcy + 3.0f;                                            \
/*                                                                        \
 * d( x, y ) = delta F( x, y )                                            \
 */                                                                       \
	d = A * _2fcx1 * _2fcx1 * 0.25f + B * _2fcx1 * fcy1 +             \
	    C * fcy1 * fcy1 - D;                                          \
/*                                                                        \
 * dinside( x, y ) - delta d( x, y ) with north direction                 \
 */                                                                       \
	dinside = B * _2fcx1 + C * _2fcy3;                                \
/*                                                                        \
 * doutside( x, y ) - delta d( x, y ) with north - east direction         \
 */                                                                       \
	doutside = A * fcx21 + B * (_2fcx + _2fcy + 5.0f) + C * _2fcy3;   \
/*                                                                        \
 * gradi( x, y ) = i coordinate of grad F( x, y )                         \
 */                                                                       \
	gradi = B * fcy21 + A * _2fcx1;                                   \
	while (cy < stop1y) {                                             \
	                                                                  \
/*                                                                        \
 * midpoint is inside - go north                                          \
 */                                                                       \
	    if (d < 0.0f || gradi > 0.0f) {                               \
		line0 -= stride;                                          \
		line1 += stride;                                          \
		d += dinside;                                             \
		dinside += d1n_n;                                         \
		doutside += d1ne_n;                                       \
		gradi += grad1n_i;                                        \
		cy++;                                                     \
	    } else                                                        \
/*                                                                        \
 * midpoint outside - go north - east                                     \
 */                                                                       \
	    {                                                             \
		line0 -= stride - 1;                                      \
		line1 += stride - 1;                                      \
		d += doutside;                                            \
		dinside += d1n_ne;                                        \
		doutside += d1ne_ne;                                      \
		gradi += grad1ne_i;                                       \
		cy++;                                                     \
		cx++;                                                     \
	    }                                                             \
	    if (cy >= midy[next_point] && next_point < 2) {               \
		if (cx >= midx[next_point]) {                             \
		    zone ^= 1;                                            \
		    next_point++;                                         \
		}                                                         \
	    }                                                             \
	                                                                  \
/*                                                                        \
 * Here goes an appropriate procedure of putting a point                  \
 */                                                                       \
	                                                                  \
/*                                                                        \
 * May be with clipping, maybe not                                        \
 */                                                                       \


/* *********************************************************** */

#define	ROTATED_ELLIPSE_PHASE2_START                                     \
	                                                                 \
/*                                                                       \
 * zone 2: x = x + 1, y = y + 1 / 2                                      \
 */                                                                      \
	fcx = cx;                                                        \
	fcy = cy;                                                        \
	_2fcx = fcx * 2.0f;                                              \
	_2fcy = fcy * 2.0f;                                              \
	fcx1 = fcx + 1.0f;                                               \
	fcy1 = fcy + 1.0f;                                               \
	fcx21 = fcx1 * 2.0f;                                             \
	fcy21 = fcy1 * 2.0f;                                             \
	_2fcx3 = _2fcx + 3.0f;                                           \
	_2fcy1 = _2fcy + 1.0f;                                           \
/*                                                                       \
 * d( x, y ) = delta F( x, y )                                           \
 */                                                                      \
	d = A * fcx1 * fcx1 + B * fcx1 * _2fcy1 +                        \
	    C * _2fcy1 * _2fcy1 * 0.25f - D;                             \
/*                                                                       \
 * dinside( x, y ) - delta d( x, y ) with north - east direction         \
 */                                                                      \
	dinside = A * _2fcx3 + B * (_2fcx + _2fcy + 5.0f) + C * fcy21;   \
/*                                                                       \
 * doutside( x, y ) - delta d( x, y ) with east direction                \
 */                                                                      \
	doutside = A * _2fcx3 + B * _2fcy1;                              \
/*                                                                       \
 * gradj( x, y ) = j coordinate of grad F( x, y )                        \
 */                                                                      \
	gradj = C * _2fcy1 + B * fcx21;                                  \
	while (cx < maxyx) {                                             \
	                                                                 \
/*                                                                       \
 * midpoint is inside - go north - east                                  \
 */                                                                      \
	    if (d < 0.0f || gradj < 0.0f) {                              \
		line0 -= stride - 1;                                     \
		line1 += stride - 1;                                     \
		d += dinside;                                            \
		dinside += d2ne_ne;                                      \
		doutside += d2e_ne;                                      \
		gradj += grad2ne_j;                                      \
		cx++;                                                    \
		cy++;                                                    \
	    } else                                                       \
/*                                                                       \
 * midpoint is outside - go east                                         \
 */                                                                      \
	    {                                                            \
		line0++;                                                 \
		line1--;                                                 \
		d += doutside;                                           \
		dinside += d2ne_e;                                       \
		doutside += d2e_e;                                       \
		gradj += grad2e_j;                                       \
		cx++;                                                    \
	    }                                                            \
	    if (cx >= midx[next_point] && next_point < 2) {              \
		zone ^= 1;                                               \
		next_point++;                                            \
	    }                                                            \
	                                                                 \
/*                                                                       \
 * Here goes an appropriate procedure of putting a point                 \
 */                                                                      \
	                                                                 \
/*                                                                       \
 * May be with clipping, maybe not                                       \
 */                                                                      \


/* *********************************************************** */

#define	ROTATED_ELLIPSE_PHASE3_START                                      \
	                                                                  \
/*                                                                        \
 * zone 3: x = x + 1, y = y - 1 / 2                                       \
 */                                                                       \
	fcx = cx;                                                         \
	fcy = cy;                                                         \
	_2fcx = fcx * 2.0f;                                               \
	_2fcy = fcy * 2.0f;                                               \
	fcx1 = fcx + 1.0f;                                                \
	fcy1 = fcy - 1.0f;                                                \
	fcx21 = fcx1 * 2.0f;                                              \
	fcy21 = fcy1 * 2.0f;                                              \
	_2fcx3 = _2fcx + 3.0f;                                            \
	_2fcy1 = _2fcy - 1.0f;                                            \
/*                                                                        \
 * d( x, y ) = delta F( x, y )                                            \
 */                                                                       \
	d = A * fcx1 * fcx1 + B * fcx1 * _2fcy1 +                         \
	    C * _2fcy1 * _2fcy1 * 0.25f - D;                              \
/*                                                                        \
 * dinside( x, y ) - delta d( x, y ) with east direction                  \
 */                                                                       \
	dinside = A * _2fcx3 + B * _2fcy1;                                \
/*                                                                        \
 * doutside( x, y ) - delta d( x, y ) with south - east direction         \
 */                                                                       \
	doutside = A * _2fcx3 + B * (_2fcy - _2fcx - 5.0f) - C * fcy21;   \
/*                                                                        \
 * gradj( x, y ) = j coordinate of grad F( x, y )                         \
 */                                                                       \
	gradj = C * _2fcy1 + B * fcx21;                                   \
	while (cx < stop3x) {                                             \
	                                                                  \
/*                                                                        \
 * midpoint is inside - go east                                           \
 */                                                                       \
	    if (d < 0.0f || gradj < 0.0f) {                               \
		line0++;                                                  \
		line1--;                                                  \
		d += dinside;                                             \
		dinside += d3e_e;                                         \
		doutside += d3se_e;                                       \
		gradj += grad3e_j;                                        \
		cx++;                                                     \
	    } else                                                        \
/*                                                                        \
 * midpoint is outside - go south east                                    \
 */                                                                       \
	    {                                                             \
		line0 += stride + 1;                                      \
		line1 -= stride + 1;                                      \
		d += doutside;                                            \
		dinside += d3e_se;                                        \
		doutside += d3se_se;                                      \
		gradj += grad3se_j;                                       \
		cy--;                                                     \
		cx++;                                                     \
	    }                                                             \
	    if (cx >= midx[next_point] && next_point < 2) {               \
		zone ^= 1;                                                \
		next_point++;                                             \
	    }                                                             \
	                                                                  \
/*                                                                        \
 * Here goes an appropriate procedure of putting a point                  \
 */                                                                       \
	                                                                  \
/*                                                                        \
 * May be with clipping, maybe not                                        \
 */                                                                       \


/* *********************************************************** */

#define	ROTATED_ELLIPSE_PHASE4_START                                     \
	                                                                 \
/*                                                                       \
 * zone 4: x = x + 1 / 2, y = y - 1                                      \
 */                                                                      \
	fcx = cx;                                                        \
	fcy = cy;                                                        \
	_2fcx = fcx * 2.0f;                                              \
	_2fcy = fcy * 2.0f;                                              \
	fcx1 = fcx + 1.0f;                                               \
	fcy1 = fcy - 1.0f;                                               \
	fcx21 = fcx1 * 2.0f;                                             \
	fcy21 = fcy1 * 2.0f;                                             \
	_2fcx1 = _2fcx + 1.0f;                                           \
	_2fcy3 = _2fcy - 3.0f;                                           \
/*                                                                       \
 * d( x, y ) = delta F( x, y )                                           \
 */                                                                      \
	d = A * _2fcx1 * _2fcx1 * 0.25f + B * _2fcx1 * fcy1 +            \
	    C * fcy1 * fcy1 - D;                                         \
/*                                                                       \
 * dinside( x, y ) - delta d( x, y ) with south - east direction         \
 */                                                                      \
	dinside = A * fcx21 + B * (_2fcy - _2fcx - 5.0f) - C * _2fcy3;   \
/*                                                                       \
 * doutside( x, y ) - delta d( x, y ) with south direction               \
 */                                                                      \
	doutside = -B * _2fcx1 - C * _2fcy3;                             \
/*                                                                       \
 * gradi( x, y ) = i coordinate of grad F( x, y )                        \
 */                                                                      \
	gradi = B * fcy21 + A * _2fcx1;                                  \
	while (cy > maxxy) {                                             \
	                                                                 \
/*                                                                       \
 * midpoint is inside - go south - east                                  \
 */                                                                      \
	    if (d < 0.0f || gradi < 0.0f) {                              \
		line0 += stride + 1;                                     \
		line1 -= stride + 1;                                     \
		d += dinside;                                            \
		dinside += d4se_se;                                      \
		doutside += d4s_se;                                      \
		gradi += grad4se_i;                                      \
		cx++;                                                    \
		cy--;                                                    \
	    } else                                                       \
/*                                                                       \
 * midpoint is outside - go south                                        \
 */                                                                      \
	    {                                                            \
		line0 += stride;                                         \
		line1 -= stride;                                         \
		d += doutside;                                           \
		dinside += d4se_s;                                       \
		doutside += d4s_s;                                       \
		gradi += grad4s_i;                                       \
		cy--;                                                    \
	    }                                                            \
	    if (cy <= midy[next_point] && next_point < 2) {              \
		if (cx >= midx[next_point]) {                            \
		    zone ^= 1;                                           \
		    next_point++;                                        \
		}                                                        \
	    }                                                            \
	                                                                 \
/*                                                                       \
 * Here goes an appropriate procedure of putting a point                 \
 */                                                                      \
	                                                                 \
/*                                                                       \
 * May be with clipping, maybe not                                       \
 */                                                                      \


/* *********************************************************** */

#define	SIMPLE_ELLIPSE_PHASE1_START                                 \
	cx1 = cx + 1;                                               \
	cy1 = cy - 1;                                               \
	_2cx = cx * 2;                                              \
	_2cy = cy * 2;                                              \
	cx21 = cx1 * 2;                                             \
	cy21 = cy1 * 2;                                             \
	_2cx3 = _2cx + 3;                                           \
	_2cy1 = _2cy - 1;                                           \
/*                                                                  \
 * d( x, y ) = delta F( x, y )                                      \
 */                                                                 \
	d = (4 * b2 - 4 * a2 * b + a2) / 4;                         \
/*                                                                  \
 * dinside( x, y ) - delta d( x, y ) with east direction            \
 */                                                                 \
	dinside = b2 * _2cx3;                                       \
/*                                                                  \
 * doutside( x, y ) - delta d( x, y ) with south - east direction   \
 */                                                                 \
	doutside = b2 * _2cx3 - a2 * cy21;                          \
/*                                                                  \
 * gradj( x, y ) = j coordinate of grad F( x, y )                   \
 */                                                                 \
	gradj = a2 * _2cy1;                                         \
	while (cx < gradstop1) {                                    \
	                                                            \
/*                                                                  \
 * midpoint is inside - go east                                     \
 */                                                                 \
	    if (d < 0 || gradj <= 0) {                              \
		d += dinside;                                       \
		dinside += d1e_e;                                   \
		doutside += d1se_e;                                 \
		cx++;                                               \
	    } else                                                  \
/*                                                                  \
 * midpoint is outside - go south - east                            \
 */                                                                 \
	    {                                                       \
		d += doutside;                                      \
		dinside += d1e_se;                                  \
		doutside += d1se_se;                                \
		cx++;                                               \
		cy--;                                               \
		line0 += stride;                                    \
		line1 -= stride;                                    \
		gradj += grad1_j;                                   \
	    }                                                       \
	                                                            \
/*                                                                  \
 * Here goes an appropriate procedure of putting a point            \
 */                                                                 \
	                                                            \
/*                                                                  \
 * May be with clipping, maybe not                                  \
 */                                                                 \


/* *********************************************************** */

#define	SIMPLE_ELLIPSE_PHASE2_START                                \
	cx1 = cx + 1;                                              \
	cy1 = cy - 1;                                              \
	_2cx = cx * 2;                                             \
	_2cy = cy * 2;                                             \
	cx21 = cx1 * 2;                                            \
	cy21 = cy1 * 2;                                            \
	_2cx1 = _2cx + 1;                                          \
	_2cy3 = _2cy - 3;                                          \
/*                                                                 \
 * d( x, y ) = delta F( x, y )                                     \
 */                                                                \
	d = (b2 * _2cx1 * _2cx1 + 4.0f * a2 * cy1 * cy1 -          \
	    4.0f * a2 * b2) / 4;                                   \
/*                                                                 \
 * dinside( x, y ) - delta d( x, y ) with south - east direction   \
 */                                                                \
	dinside = b2 * cx21 - a2 * _2cy3;                          \
/*                                                                 \
 * doutside( x, y ) - delta d( x, y ) with south direction         \
 */                                                                \
	doutside = -a2 * _2cy3;                                    \
/*                                                                 \
 * gradi( x, y ) = i coordinate of grad F( x, y )                  \
 */                                                                \
	gradi = b2 * _2cx1;                                        \
	while (cy > 0) {                                           \
	                                                           \
/*                                                                 \
 * midpoint is inside - go south - east                            \
 */                                                                \
	    if (d < 0 || gradi <= 0) {                             \
		d += dinside;                                      \
		dinside += d2se_se;                                \
		doutside += d2s_se;                                \
		gradi += grad2_i;                                  \
		cx++;                                              \
		cy--;                                              \
	    } else                                                 \
/*                                                                 \
 * midpoint is outside - go south                                  \
 */                                                                \
	    {                                                      \
		d += doutside;                                     \
		dinside += d2se_s;                                 \
		doutside += d2s_s;                                 \
		cy--;                                              \
	    }                                                      \
	    line0 += stride;                                       \
	    line1 -= stride;                                       \
	                                                           \
/*                                                                 \
 * Here goes an appropriate procedure of putting a point           \
 */                                                                \
	                                                           \
/*                                                                 \
 * May be with clipping, maybe not                                 \
 */                                                                \


/* *********************************************************** */

mlib_status
__mlib_GraphicsDrawEllipse_A_8(
    mlib_image *buffer,
    mlib_s16 x,
    mlib_s16 y,
    mlib_s32 a,
    mlib_s32 b,
    mlib_f32 t,
    mlib_s32 c)
{
	mlib_s32 stride = mlib_ImageGetStride(buffer);
	mlib_s32 width = mlib_ImageGetWidth(buffer);
	mlib_s32 height = mlib_ImageGetHeight(buffer);
	mlib_u8 *data = mlib_ImageGetData(buffer);
	mlib_u8 *line0, *line1;
	mlib_f32 cosfi, sinfi, cosfi2, sinfi2;
	mlib_s32 doclip = 0;
	mlib_s32 left, right, bottom, top, length;
	mlib_s32 cx, cy;
	mlib_f32 a2, b2;
	mlib_s32 zeroangle = 0;
	mlib_f32 gradstopx, gradstopy;
	mlib_f32 s, sc;
	mlib_f32 ab, ba;
	mlib_f32 fcx, fcy;
	mlib_f32 x0, y0, find0, ix2, iy2, l, zz;
	mlib_s32 ind0, ind1, ind2;
	mlib_d64 coef0, coef1, coef2;
	mlib_d64 ccoef0, ccoef1, ccoef2;
	mlib_d64 d_one = ((mlib_d64 *)mlib_v_TabAlias)[0];
	mlib_f32 cf;
	mlib_s32 flag;

	if (!data)
		return (MLIB_NULLPOINTER);

	if (a < 0 || b < 0)
		return (MLIB_FAILURE);

	mlib_sincosf(-t, &sinfi, &cosfi);

	if (a == 0 || b == 0) {
		mlib_s32 x1, y1, x2, y2;

		if (b == 0) {
			x1 = a * cosfi + ((cosfi > 0.0f) ? 0.5f : -0.5f);
			y1 = -a * sinfi + ((sinfi < 0.0f) ? 0.5f : -0.5f);
			x2 = -x1;
			y2 = -y1;
		} else {
			x1 = -b * sinfi + ((sinfi < 0.0f) ? 0.5f : -0.5f);
			y1 = -b * cosfi + ((cosfi < 0.0f) ? 0.5f : -0.5f);
			x2 = -x1;
			y2 = -y1;
		}

		return __mlib_GraphicsDrawLine_A_8(buffer, x + x1, y + y1,
		    x + x2, y + y2, c);
	}

	if (a == b)
		return (__mlib_GraphicsDrawCircle_A_8(buffer, x, y, a, c));

	length = max(a, b);

	if (mlib_fabs(sinfi * length) < 0.5f)
		zeroangle = 1;
	else if (mlib_fabs(cosfi * length) < 0.5f) {
		mlib_s32 tmp = b;

		b = a;
		a = tmp;
		zeroangle = 1;
	}

	vis_write_gsr((1 << 3) + 0);
	cf = vis_to_float(c);

	a2 = a * a;
	b2 = b * b;
	ab = ((mlib_f32)a) / ((mlib_f32)b);
	ba = ((mlib_f32)b) / ((mlib_f32)a);

	if (!zeroangle) {
		mlib_f32 A, B, C, D, A2, B2, C2, _2A, _2B, _2C, _4B, s, z1, z2;
		mlib_f32 d, dinside, doutside;
		mlib_f32 gradi, gradj;
		mlib_f32 d1n_n, d1n_ne, d1ne_n, d1ne_ne;
		mlib_f32 d2e_e, d2e_ne, d2ne_e, d2ne_ne;
		mlib_f32 d3e_e, d3e_se, d3se_e, d3se_se;
		mlib_f32 d4s_s, d4s_se, d4se_s, d4se_se;
		mlib_f32 grad1n_i, grad1ne_i;
		mlib_f32 grad2e_j, grad2ne_j;
		mlib_f32 grad3e_j, grad3se_j;
		mlib_f32 grad4s_i, grad4se_i;
		mlib_s32 stop1y, stop3x;
		mlib_f32 _2fcx, _2fcy, fcx1, fcy1, fcx21, fcy21, _2fcx1, _2fcy1,
		    _2fcx3, _2fcy3;
		mlib_s32 minxx, minxy, maxyx, maxyy, maxxx, maxxy, minyx, minyy;
		mlib_f32 fminxx, fminxy, fmaxxx, fmaxxy;
		mlib_s32 change_points[2], next_point, i, zone;
		mlib_f32 fmidx[4], fmidy[4];
		mlib_s32 midx[3], midy[3];

		cosfi2 = cosfi * cosfi;
		sinfi2 = sinfi * sinfi;
		A = b2 * cosfi2 + a2 * sinfi2;
		B = sinfi * cosfi * (b2 - a2);
		C = b2 * sinfi2 + a2 * cosfi2;
		D = a2 * b2;
		_2A = A * 2.0f;
		_2B = B * 2.0f;
		_2C = C * 2.0f;
		_4B = B * 4.0f;
		A2 = A * A;
		B2 = B * B;
		C2 = C * C;

/* determine the rectangle that the the ellipse fits in */
		s = mlib_sqrtf(D / (A * C2 - C * B2));
		fminxx = -C * s;
		minxx = fminxx - 0.5f;
		fminxy = B * s;
		minxy = fminxy + ((fminxy > 0.0f) ? 0.5f : -0.5f);
		fmaxxx = -fminxx;
		fmaxxy = -fminxy;
		maxxx = -minxx;
		maxxy = -minxy;
		s = mlib_sqrtf(D / (A2 * C - B2 * A));
		minyy = -A * s - 0.5f;
		minyx = B * s + ((B > 0.0f) ? 0.5f : -0.5f);
		maxyy = -minyy;
		maxyx = -minyx;

		s = mlib_sqrtf(a2 + b2);
		x0 = a2 / s;
		ix2 = -x0;
		y0 = b2 / s;
		iy2 = -y0;
/* right - top */
		fmidx[0] = x0 * cosfi - y0 * sinfi;
/* right - bottom */
		fmidx[1] = x0 * cosfi - iy2 * sinfi;
/* left - top */
		fmidx[2] = ix2 * cosfi - y0 * sinfi;
/* left - bottom */
		fmidx[3] = ix2 * cosfi - iy2 * sinfi;
/* right - top */
		fmidy[0] = x0 * sinfi + y0 * cosfi;
/* right - bottom */
		fmidy[1] = x0 * sinfi + iy2 * cosfi;
/* left - top */
		fmidy[2] = ix2 * sinfi + y0 * cosfi;
/* left - bottom */
		fmidy[3] = ix2 * sinfi + iy2 * cosfi;
		gradstopx = x0;
		gradstopy = y0;
		x0 = SQRT2_256 / gradstopx;
		y0 = SQRT2_256 / gradstopy;
		sc = gradstopx - gradstopy;

		next_point = 0;
		for (i = 0; i < 4; i++) {
			if ((fmidy[i] - fminxy) * (fmaxxx - fminxx) >=
			    (fmidx[i] - fminxx) * (fmaxxy - fminxy))
				change_points[next_point++] = i;

			if (next_point > 1)
				break;
		}

		if (next_point == 1) {
			i = (change_points[0] + 1) & 3;
			fmidx[i] = maxxx;
			fmidy[i] = maxxy;
			change_points[1] = i;
		}

		if (fmidx[change_points[0]] > fmidx[change_points[1]]) {
			i = change_points[0];
			change_points[0] = change_points[1];
			change_points[1] = i;
		}

		next_point = 0;

		if (change_points[0] == 1 || change_points[0] == 2)
/* we are in the zone of horizontal semiaxis */
			zone = 0;
		else
/* we are in the zone of vertical semiaxis */
			zone = 1;

		i = change_points[0];
		midx[0] = (fmidx[i] > 0.0f) ? fmidx[i] + 0.5f : fmidx[i] - 0.5f;
		midy[0] = (fmidy[i] > 0.0f) ? fmidy[i] + 0.5f : fmidy[i] - 0.5f;
		i = change_points[1];
		midx[1] = (fmidx[i] > 0.0f) ? fmidx[i] + 0.5f : fmidx[i] - 0.5f;
		midy[1] = (fmidy[i] > 0.0f) ? fmidy[i] + 0.5f : fmidy[i] - 0.5f;

		z1 = C + B;
		z2 = A + B;
		s = mlib_sqrtf(D / (A * z1 * z1 - 2.0f * B * z1 * z2 +
		    C * z2 * z2));
		stop1y = (z2 > 0.0f) ? (z2 * s + 0.5f) : (z2 * s - 0.5f);
		z1 = C - B;
		z2 = A - B;
		s = mlib_sqrtf(D / (A * z1 * z1 + 2.0f * B * z1 * z2 +
		    C * z2 * z2));
		stop3x = (z1 > 0.0f) ? (z1 * s + 0.5f) : (z1 * s - 0.5f);

		left = minxx + x - 1;
		right = maxxx + x + 1;
		bottom = minyy + y - 1;
		top = maxyy + y + 1;

		if (right < 0 || left >= width || top < 0 || bottom >= height)
			return (MLIB_SUCCESS);

		if (left < 0 || right >= width || bottom < 0 || top >= height)
			doclip = 1;

		cx = minxx;
		cy = minxy;
		line0 = data + stride * (y - minxy) + (x + minxx);
		line1 = data + stride * (y + minxy) + (x - minxx);

		d4s_s = d1n_n = _2C;
		d1ne_n = d1n_ne = _2B + _2C;
		d2ne_ne = d1ne_ne = _2A + _4B + _2C;
		d3e_e = d2e_e = _2A;
		d2ne_e = d2e_ne = _2A + _2B;
		d3se_e = d3e_se = _2A - _2B;
		d4se_se = d3se_se = _2A - _4B + _2C;
		d4se_s = d4s_se = _2C - _2B;

		grad3e_j = grad2e_j = grad1n_i = _2B;
		grad1ne_i = _2A + _2B;
		grad2ne_j = _2C + _2B;
		grad3se_j = _2B - _2C;
		grad4s_i = -_2B;
		grad4se_i = _2A - _2B;

		if (!doclip) {
			ROTATED_ELLIPSE_PHASE1_START;
			ROTATED_CALCULATE_DISTANCE;
			MAKE_3_COEF;
			ROTATED_PUT_NOCLIP_X(8);
			PHASE_END;

			flag = 1;
			ROTATED_ELLIPSE_PHASE2_START;
			ROTATED_CALCULATE_DISTANCE;
			MAKE_3_COEF;

			if (flag) {
				flag = 0;
				coef2 = coef1;
				ccoef2 = ccoef1;
				ROTATED_PUT_NOCLIP_CORNER(8, -1);
			} else {
				ROTATED_PUT_NOCLIP_Y(8);
			}

			PHASE_END;

			ROTATED_ELLIPSE_PHASE3_START;
			ROTATED_CALCULATE_DISTANCE;
			MAKE_3_COEF;
			ROTATED_PUT_NOCLIP_Y(8);
			PHASE_END;

			flag = 1;
			ROTATED_ELLIPSE_PHASE4_START;
			ROTATED_CALCULATE_DISTANCE;
			MAKE_3_COEF_REVERSE;

			if (flag) {
				flag = 0;
				coef1 = coef2;
				ccoef1 = ccoef2;
				ROTATED_PUT_NOCLIP_CORNER(8, 1);
			} else {
				ROTATED_PUT_NOCLIP_X(8);
			}

			PHASE_END;
		} else {
			ROTATED_ELLIPSE_PHASE1_START;
			ROTATED_CALCULATE_DISTANCE;
			MAKE_3_COEF;
			ROTATED_PUT_CLIP_X(8);
			PHASE_END;

			flag = 1;
			ROTATED_ELLIPSE_PHASE2_START;
			ROTATED_CALCULATE_DISTANCE;
			MAKE_3_COEF;

			if (flag) {
				flag = 0;
				coef2 = coef1;
				ccoef2 = ccoef1;
				ROTATED_PUT_CLIP_CORNER(8, -1);
			} else {
				ROTATED_PUT_CLIP_Y(8);
			}

			PHASE_END;

			ROTATED_ELLIPSE_PHASE3_START;
			ROTATED_CALCULATE_DISTANCE;
			MAKE_3_COEF;
			ROTATED_PUT_CLIP_Y(8);
			PHASE_END;

			flag = 1;
			ROTATED_ELLIPSE_PHASE4_START;
			ROTATED_CALCULATE_DISTANCE;
			MAKE_3_COEF_REVERSE;

			if (flag) {
				flag = 0;
				coef1 = coef2;
				ccoef1 = ccoef2;
				ROTATED_PUT_CLIP_CORNER(8, 1);
			} else {
				ROTATED_PUT_CLIP_X(8);
			}

			PHASE_END;
		}
	} else {
/* Simple algorithm that draws a simple not rotated ellipse */

		mlib_s32 _2a2, _2b2;
		mlib_s32 cx1, cy1, _2cx, _2cy, _2cx1, _2cy1, _2cx3, _2cy3, cx21,
		    cy21;
		mlib_s32 d, dinside, doutside;
		mlib_s32 gradi, gradj;
		mlib_s32 d1e_e, d1e_se, d1se_e, d1se_se;
		mlib_s32 d2s_s, d2s_se, d2se_s, d2se_se;
		mlib_s32 grad1_j, grad2_i;
		mlib_s32 gradstop1;

		left = x - a - 1;
		right = x + a + 1;
		top = y + b + 1;
		bottom = y - b - 1;

		if (right < 0 || left >= width || top < 0 || bottom >= height)
			return (MLIB_SUCCESS);

		if (left < 0 || right >= width || bottom < 0 || top >= height)
			doclip = 1;

		_2a2 = a2 * 2;
		_2b2 = b2 * 2;

		s = mlib_sqrtf(a2 + b2);
		gradstop1 = a2 / s + 0.5f;
		gradstopx = a2 / s;
		gradstopy = b2 / s;
		x0 = SQRT2_256 / gradstopx;
		y0 = SQRT2_256 / gradstopy;
		sc = gradstopx - gradstopy;

		cx = 0;
		cy = b;
		line0 = data + stride * (y - b) + x;
		line1 = data + stride * (y + b) + x;

		d1se_e = d1e_se = d1e_e = _2b2;
		d2se_se = d1se_se = _2b2 + _2a2;
		d2se_s = d2s_se = d2s_s = _2a2;

		grad1_j = -_2a2;
		grad2_i = _2b2;

		if (!doclip) {
			SIMPLE_CALCULATE_DISTANCE_Y;
			MAKE_3_COEF;
			SIMPLE_PUT_NOCLIP_Y(8);

			SIMPLE_ELLIPSE_PHASE1_START;
			SIMPLE_CALCULATE_DISTANCE_Y;
			MAKE_3_COEF;
			SIMPLE_PUT_NOCLIP_Y(8);
			PHASE_END;

			SIMPLE_CALCULATE_DISTANCE_MIDDLE;
			coef0 = LDCOEFD(ind0);
			ccoef0 = CALC_CCOEF(coef0);
			SIMPLE_PUT_NOCLIP_MIDDLE(8);
			flag = 0;

			SIMPLE_ELLIPSE_PHASE2_START;
			SIMPLE_CALCULATE_DISTANCE_X;

			if (!flag) {
				ind2 = 384;
				flag = 1;
			}

			MAKE_3_COEF;
			SIMPLE_PUT_NOCLIP_X(8);
			PHASE_END;
		} else {
			SIMPLE_CALCULATE_DISTANCE_Y;
			MAKE_3_COEF;
			SIMPLE_PUT_CLIP_Y(8);

			SIMPLE_ELLIPSE_PHASE1_START;
			SIMPLE_CALCULATE_DISTANCE_Y;
			MAKE_3_COEF;
			SIMPLE_PUT_CLIP_Y(8);
			PHASE_END;

			SIMPLE_CALCULATE_DISTANCE_MIDDLE;
			coef0 = LDCOEFD(ind0);
			ccoef0 = CALC_CCOEF(coef0);
			SIMPLE_PUT_CLIP_MIDDLE(8);
			flag = 0;

			SIMPLE_ELLIPSE_PHASE2_START;
			SIMPLE_CALCULATE_DISTANCE_X;

			if (!flag) {
				ind2 = 384;
				flag = 1;
			}

			MAKE_3_COEF;
			SIMPLE_PUT_CLIP_X(8);
			PHASE_END;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsDrawEllipse_A_32(
    mlib_image *buffer,
    mlib_s16 x,
    mlib_s16 y,
    mlib_s32 a,
    mlib_s32 b,
    mlib_f32 t,
    mlib_s32 c)
{
	mlib_s32 stride = mlib_ImageGetStride(buffer) / 4;
	mlib_s32 width = mlib_ImageGetWidth(buffer);
	mlib_s32 height = mlib_ImageGetHeight(buffer);
	mlib_s32 *data = mlib_ImageGetData(buffer);
	mlib_s32 *line0, *line1;
	mlib_f32 cosfi, sinfi, cosfi2, sinfi2;
	mlib_s32 doclip = 0;
	mlib_s32 left, right, bottom, top, length;
	mlib_s32 cx, cy;
	mlib_f32 a2, b2;
	mlib_s32 zeroangle = 0;
	mlib_f32 s, sc;
	mlib_f32 ab, ba;
	mlib_f32 fcx, fcy;
	mlib_f32 x0, y0, find0, ix2, iy2, l, zz;
	mlib_f32 gradstopx, gradstopy;
	mlib_s32 ind0, ind1, ind2;
	mlib_d64 coef0, coef1, coef2;
	mlib_d64 ccoef0, ccoef1, ccoef2;
	mlib_d64 d_one = ((mlib_d64 *)mlib_v_TabAlias)[0];
	mlib_f32 cf;
	mlib_s32 flag;

	if (!data)
		return (MLIB_NULLPOINTER);

	if (a < 0 || b < 0)
		return (MLIB_FAILURE);

	mlib_sincosf(-t, &sinfi, &cosfi);

	if (a == 0 || b == 0) {
		mlib_s32 x1, y1, x2, y2;

		if (b == 0) {
			x1 = a * cosfi + ((cosfi > 0.0f) ? 0.5f : -0.5f);
			y1 = -a * sinfi + ((sinfi < 0.0f) ? 0.5f : -0.5f);
			x2 = -x1;
			y2 = -y1;
		} else {
			x1 = -b * sinfi + ((sinfi < 0.0f) ? 0.5f : -0.5f);
			y1 = -b * cosfi + ((cosfi < 0.0f) ? 0.5f : -0.5f);
			x2 = -x1;
			y2 = -y1;
		}

		return __mlib_GraphicsDrawLine_A_32(buffer, x + x1, y + y1,
		    x + x2, y + y2, c);
	}

	if (a == b)
		return (__mlib_GraphicsDrawCircle_A_32(buffer, x, y, a, c));

	length = max(a, b);

	if (mlib_fabs(sinfi * length) < 0.5f)
		zeroangle = 1;
	else if (mlib_fabs(cosfi * length) < 0.5f) {
		mlib_s32 tmp = b;

		b = a;
		a = tmp;
		zeroangle = 1;
	}

	vis_write_gsr((1 << 3) + 0);
	cf = vis_to_float(c);

	a2 = a * a;
	b2 = b * b;
	ab = ((mlib_f32)a) / ((mlib_f32)b);
	ba = ((mlib_f32)b) / ((mlib_f32)a);

	if (!zeroangle) {
		mlib_f32 A, B, C, D, A2, B2, C2, _2A, _2B, _2C, _4B, s, z1, z2;
		mlib_f32 d, dinside, doutside;
		mlib_f32 gradi, gradj;
		mlib_f32 d1n_n, d1n_ne, d1ne_n, d1ne_ne;
		mlib_f32 d2e_e, d2e_ne, d2ne_e, d2ne_ne;
		mlib_f32 d3e_e, d3e_se, d3se_e, d3se_se;
		mlib_f32 d4s_s, d4s_se, d4se_s, d4se_se;
		mlib_f32 grad1n_i, grad1ne_i;
		mlib_f32 grad2e_j, grad2ne_j;
		mlib_f32 grad3e_j, grad3se_j;
		mlib_f32 grad4s_i, grad4se_i;
		mlib_s32 stop1y, stop3x;
		mlib_f32 _2fcx, _2fcy, fcx1, fcy1, fcx21, fcy21, _2fcx1, _2fcy1,
		    _2fcx3, _2fcy3;
		mlib_s32 minxx, minxy, maxyx, maxyy, maxxx, maxxy, minyx, minyy;
		mlib_f32 fminxx, fminxy, fmaxxx, fmaxxy;
		mlib_s32 change_points[2], next_point, i, zone;
		mlib_f32 fmidx[4], fmidy[4];
		mlib_s32 midx[3], midy[3];

		cosfi2 = cosfi * cosfi;
		sinfi2 = sinfi * sinfi;
		A = b2 * cosfi2 + a2 * sinfi2;
		B = sinfi * cosfi * (b2 - a2);
		C = b2 * sinfi2 + a2 * cosfi2;
		D = a2 * b2;
		_2A = A * 2.0f;
		_2B = B * 2.0f;
		_2C = C * 2.0f;
		_4B = B * 4.0f;
		A2 = A * A;
		B2 = B * B;
		C2 = C * C;

/* determine the rectangle that the the ellipse fits in */
		s = mlib_sqrtf(D / (A * C2 - C * B2));
		fminxx = -C * s;
		minxx = fminxx - 0.5f;
		fminxy = B * s;
		minxy = fminxy + ((fminxy > 0.0f) ? 0.5f : -0.5f);
		fmaxxx = -fminxx;
		fmaxxy = -fminxy;
		maxxx = -minxx;
		maxxy = -minxy;
		s = mlib_sqrtf(D / (A2 * C - B2 * A));
		minyy = -A * s - 0.5f;
		minyx = B * s + ((B > 0.0f) ? 0.5f : -0.5f);
		maxyy = -minyy;
		maxyx = -minyx;

		s = mlib_sqrtf(a2 + b2);
		x0 = a2 / s;
		ix2 = -x0;
		y0 = b2 / s;
		iy2 = -y0;
/* right - top */
		fmidx[0] = x0 * cosfi - y0 * sinfi;
/* right - bottom */
		fmidx[1] = x0 * cosfi - iy2 * sinfi;
/* left - top */
		fmidx[2] = ix2 * cosfi - y0 * sinfi;
/* left - bottom */
		fmidx[3] = ix2 * cosfi - iy2 * sinfi;
/* right - top */
		fmidy[0] = x0 * sinfi + y0 * cosfi;
/* right - bottom */
		fmidy[1] = x0 * sinfi + iy2 * cosfi;
/* left - top */
		fmidy[2] = ix2 * sinfi + y0 * cosfi;
/* left - bottom */
		fmidy[3] = ix2 * sinfi + iy2 * cosfi;
		gradstopx = x0;
		gradstopy = y0;
		x0 = SQRT2_256 / gradstopx;
		y0 = SQRT2_256 / gradstopy;
		sc = gradstopx - gradstopy;

		next_point = 0;
		for (i = 0; i < 4; i++) {
			if ((fmidy[i] - fminxy) * (fmaxxx - fminxx) >=
			    (fmidx[i] - fminxx) * (fmaxxy - fminxy))
				change_points[next_point++] = i;

			if (next_point > 1)
				break;
		}

		if (next_point == 1) {
			i = (change_points[0] + 1) & 3;
			fmidx[i] = maxxx;
			fmidy[i] = maxxy;
			change_points[1] = i;
		}

		if (fmidx[change_points[0]] > fmidx[change_points[1]]) {
			i = change_points[0];
			change_points[0] = change_points[1];
			change_points[1] = i;
		}

		next_point = 0;

		if (change_points[0] == 1 || change_points[0] == 2)
/* we are in the zone of horizontal semiaxis */
			zone = 0;
		else
/* we are in the zone of vertical semiaxis */
			zone = 1;

		i = change_points[0];
		midx[0] = (fmidx[i] > 0.0f) ? fmidx[i] + 0.5f : fmidx[i] - 0.5f;
		midy[0] = (fmidy[i] > 0.0f) ? fmidy[i] + 0.5f : fmidy[i] - 0.5f;
		i = change_points[1];
		midx[1] = (fmidx[i] > 0.0f) ? fmidx[i] + 0.5f : fmidx[i] - 0.5f;
		midy[1] = (fmidy[i] > 0.0f) ? fmidy[i] + 0.5f : fmidy[i] - 0.5f;

		z1 = C + B;
		z2 = A + B;
		s = mlib_sqrtf(D / (A * z1 * z1 - 2.0f * B * z1 * z2 +
		    C * z2 * z2));
		stop1y = (z2 > 0.0f) ? (z2 * s + 0.5f) : (z2 * s - 0.5f);
		z1 = C - B;
		z2 = A - B;
		s = mlib_sqrtf(D / (A * z1 * z1 + 2.0f * B * z1 * z2 +
		    C * z2 * z2));
		stop3x = (z1 > 0.0f) ? (z1 * s + 0.5f) : (z1 * s - 0.5f);

		left = minxx + x - 1;
		right = maxxx + x + 1;
		bottom = minyy + y - 1;
		top = maxyy + y + 1;

		if (right < 0 || left >= width || top < 0 || bottom >= height)
			return (MLIB_SUCCESS);

		if (left < 0 || right >= width || bottom < 0 || top >= height)
			doclip = 1;

		cx = minxx;
		cy = minxy;
		line0 = data + stride * (y - minxy) + (x + minxx);
		line1 = data + stride * (y + minxy) + (x - minxx);

		d4s_s = d1n_n = _2C;
		d1ne_n = d1n_ne = _2B + _2C;
		d2ne_ne = d1ne_ne = _2A + _4B + _2C;
		d3e_e = d2e_e = _2A;
		d2ne_e = d2e_ne = _2A + _2B;
		d3se_e = d3e_se = _2A - _2B;
		d4se_se = d3se_se = _2A - _4B + _2C;
		d4se_s = d4s_se = _2C - _2B;

		grad3e_j = grad2e_j = grad1n_i = _2B;
		grad1ne_i = _2A + _2B;
		grad2ne_j = _2C + _2B;
		grad3se_j = _2B - _2C;
		grad4s_i = -_2B;
		grad4se_i = _2A - _2B;

		if (!doclip) {
			ROTATED_ELLIPSE_PHASE1_START;
			ROTATED_CALCULATE_DISTANCE;
			MAKE_3_COEF;
			ROTATED_PUT_NOCLIP_X(32);
			PHASE_END;

			flag = 1;
			ROTATED_ELLIPSE_PHASE2_START;
			ROTATED_CALCULATE_DISTANCE;
			MAKE_3_COEF;

			if (flag) {
				flag = 0;
				coef2 = coef1;
				ccoef2 = ccoef1;
				ROTATED_PUT_NOCLIP_CORNER(32, -1);
			} else {
				ROTATED_PUT_NOCLIP_Y(32);
			}

			PHASE_END;

			ROTATED_ELLIPSE_PHASE3_START;
			ROTATED_CALCULATE_DISTANCE;
			MAKE_3_COEF;
			ROTATED_PUT_NOCLIP_Y(32);
			PHASE_END;

			flag = 1;
			ROTATED_ELLIPSE_PHASE4_START;
			ROTATED_CALCULATE_DISTANCE;
			MAKE_3_COEF_REVERSE;

			if (flag) {
				flag = 0;
				coef1 = coef2;
				ccoef1 = ccoef2;
				ROTATED_PUT_NOCLIP_CORNER(32, 1);
			} else {
				ROTATED_PUT_NOCLIP_X(32);
			}

			PHASE_END;
		} else {
			ROTATED_ELLIPSE_PHASE1_START;
			ROTATED_CALCULATE_DISTANCE;
			MAKE_3_COEF;
			ROTATED_PUT_CLIP_X(32);
			PHASE_END;

			flag = 1;
			ROTATED_ELLIPSE_PHASE2_START;
			ROTATED_CALCULATE_DISTANCE;
			MAKE_3_COEF;

			if (flag) {
				flag = 0;
				coef2 = coef1;
				ccoef2 = ccoef1;
				ROTATED_PUT_CLIP_CORNER(32, -1);
			} else {
				ROTATED_PUT_CLIP_Y(32);
			}

			PHASE_END;

			ROTATED_ELLIPSE_PHASE3_START;
			ROTATED_CALCULATE_DISTANCE;
			MAKE_3_COEF;
			ROTATED_PUT_CLIP_Y(32);
			PHASE_END;

			flag = 1;
			ROTATED_ELLIPSE_PHASE4_START;
			ROTATED_CALCULATE_DISTANCE;
			MAKE_3_COEF_REVERSE;

			if (flag) {
				flag = 0;
				coef1 = coef2;
				ccoef1 = ccoef2;
				ROTATED_PUT_CLIP_CORNER(32, 1);
			} else {
				ROTATED_PUT_CLIP_X(32);
			}

			PHASE_END;
		}
	} else {
/* Simple algorithm that draws a simple not rotated ellipse */

		mlib_s32 _2a2, _2b2;
		mlib_s32 cx1, cy1, _2cx, _2cy, _2cx1, _2cy1, _2cx3, _2cy3, cx21,
		    cy21;
		mlib_s32 d, dinside, doutside;
		mlib_s32 gradi, gradj;
		mlib_s32 d1e_e, d1e_se, d1se_e, d1se_se;
		mlib_s32 d2s_s, d2s_se, d2se_s, d2se_se;
		mlib_s32 grad1_j, grad2_i;
		mlib_s32 gradstop1;

		left = x - a - 1;
		right = x + a + 1;
		top = y + b + 1;
		bottom = y - b - 1;

		if (right < 0 || left >= width || top < 0 || bottom >= height)
			return (MLIB_SUCCESS);

		if (left < 0 || right >= width || bottom < 0 || top >= height)
			doclip = 1;

		_2a2 = a2 * 2;
		_2b2 = b2 * 2;

		ab = ((mlib_f32)a) / ((mlib_f32)b);
		ba = ((mlib_f32)b) / ((mlib_f32)a);

		s = mlib_sqrtf(a2 + b2);
		gradstop1 = a2 / s + 0.5f;
		gradstopx = a2 / s;
		gradstopy = b2 / s;
		x0 = SQRT2_256 / gradstopx;
		y0 = SQRT2_256 / gradstopy;
		sc = gradstopx - gradstopy;

		cx = 0;
		cy = b;
		line0 = data + stride * (y - b) + x;
		line1 = data + stride * (y + b) + x;

		d1se_e = d1e_se = d1e_e = _2b2;
		d2se_se = d1se_se = _2b2 + _2a2;
		d2se_s = d2s_se = d2s_s = _2a2;

		grad1_j = -_2a2;
		grad2_i = _2b2;

		if (!doclip) {
			SIMPLE_CALCULATE_DISTANCE_Y;
			MAKE_3_COEF;
			SIMPLE_PUT_NOCLIP_Y(32);

			SIMPLE_ELLIPSE_PHASE1_START;
			SIMPLE_CALCULATE_DISTANCE_Y;
			MAKE_3_COEF;
			SIMPLE_PUT_NOCLIP_Y(32);
			PHASE_END;

			SIMPLE_CALCULATE_DISTANCE_MIDDLE;
			coef0 = LDCOEFD(ind0);
			ccoef0 = CALC_CCOEF(coef0);
			SIMPLE_PUT_NOCLIP_MIDDLE(32);
			flag = 0;

			SIMPLE_ELLIPSE_PHASE2_START;
			SIMPLE_CALCULATE_DISTANCE_X;

			if (!flag) {
				ind2 = 384;
				flag = 1;
			}

			MAKE_3_COEF;
			SIMPLE_PUT_NOCLIP_X(32);
			PHASE_END;
		} else {
			SIMPLE_CALCULATE_DISTANCE_Y;
			MAKE_3_COEF;
			SIMPLE_PUT_CLIP_Y(32);

			SIMPLE_ELLIPSE_PHASE1_START;
			SIMPLE_CALCULATE_DISTANCE_Y;
			MAKE_3_COEF;
			SIMPLE_PUT_CLIP_Y(32);
			PHASE_END;

			SIMPLE_CALCULATE_DISTANCE_MIDDLE;
			coef0 = LDCOEFD(ind0);
			ccoef0 = CALC_CCOEF(coef0);
			SIMPLE_PUT_CLIP_MIDDLE(32);
			flag = 0;

			SIMPLE_ELLIPSE_PHASE2_START;
			SIMPLE_CALCULATE_DISTANCE_X;

			if (!flag) {
				ind2 = 384;
				flag = 1;
			}

			MAKE_3_COEF;
			SIMPLE_PUT_CLIP_X(32);
			PHASE_END;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
