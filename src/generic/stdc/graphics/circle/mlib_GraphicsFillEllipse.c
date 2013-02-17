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

#pragma ident	"@(#)mlib_GraphicsFillEllipse.c	9.2	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_GraphicsFillEllipse_[8 | 32] - fill ellipse
 *
 * SYNOPSIS
 *      mlib_status mlib_GraphicsFillEllipse_8 (mlib_image *buffer,
 *                                              mlib_s16   x,
 *                                              mlib_s16   y,
 *                                              mlib_s32   a,
 *                                              mlib_s32   b,
 *                                              mlib_f32   t,
 *                                              mlib_s32   c);
 *
 *      mlib_status mlib_GraphicsFillEllipse_32(mlib_image *buffer,
 *                                              mlib_s16   x,
 *                                              mlib_s16   y,
 *                                              mlib_s32   a,
 *                                              mlib_s32   b,
 *                                              mlib_f32   t,
 *                                              mlib_s32   c);
 *
 * ARGUMENTS
 *      buffer  pointer to the image which the function is filling into
 *      x, y    coordinates of the center
 *      a       major semiaxis of the ellipse
 *      b       minor semiaxis of the ellipse
 *      t       angle of major semiaxis in radians
 *      c       color used in the filling
 * DESCRIPTION
 *      Function fills an ellipse at (x, y), major semiaxis a, minor semiaxis b,
 *      and the angle of the major semiaxis is t clockwise from X axis.
 */

#include <mlib_graphics.h>
#include <mlib_SysMath.h>
#include <mlib_GraphicsFillRow.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_GraphicsFillEllipse_8 = __mlib_GraphicsFillEllipse_8
#pragma weak mlib_GraphicsFillEllipse_32 = __mlib_GraphicsFillEllipse_32

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_GraphicsFillEllipse_8) mlib_GraphicsFillEllipse_8
    __attribute__((weak, alias("__mlib_GraphicsFillEllipse_8")));
__typeof__(__mlib_GraphicsFillEllipse_32) mlib_GraphicsFillEllipse_32
    __attribute__((weak, alias("__mlib_GraphicsFillEllipse_32")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	max(a, b)	((a) < (b) ? (b) : (a))
#define	min(a, b)	((a) > (b) ? (b) : (a))

/* *********************************************************** */

#define	ROTATED_ELLIPSE_PHASE1                                            \
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
	d = A * _2fcx1 * _2fcx1 * .25f + B * _2fcx1 * fcy1 +              \
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
	buf[cy + bufsize] = cx;                                           \
	while (cy < stop1y) {                                             \
	                                                                  \
/*                                                                        \
 * midpoint is inside - go north                                          \
 */                                                                       \
	    if (d < 0.0f || gradi > 0.0f) {                               \
		d += dinside;                                             \
		dinside += d1n_n;                                         \
		doutside += d1ne_n;                                       \
		gradi += grad1n_i;                                        \
		cy++;                                                     \
	    } else {                                                      \
	                                                                  \
/*                                                                        \
 * midpoint outside - go north - east                                     \
 */                                                                       \
		d += doutside;                                            \
		dinside += d1n_ne;                                        \
		doutside += d1ne_ne;                                      \
		gradi += grad1ne_i;                                       \
		cy++;                                                     \
		cx++;                                                     \
	    }                                                             \
	    buf[cy + bufsize] = cx;                                       \
	}

/* *********************************************************** */

#define	ROTATED_ELLIPSE_PHASE2                                           \
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
	    C * _2fcy1 * _2fcy1 * .25f - D;                              \
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
		d += dinside;                                            \
		dinside += d2ne_ne;                                      \
		doutside += d2e_ne;                                      \
		gradj += grad2ne_j;                                      \
		cx++;                                                    \
		cy++;                                                    \
		buf[cy + bufsize] = cx;                                  \
	    } else {                                                     \
	                                                                 \
/*                                                                       \
 * midpoint is outside - go east                                         \
 */                                                                      \
		d += doutside;                                           \
		dinside += d2ne_e;                                       \
		doutside += d2e_e;                                       \
		gradj += grad2e_j;                                       \
		cx++;                                                    \
	    }                                                            \
	}                                                                \
	                                                                 \
	if (cy < maxyy) {                                                \
	    buf[0] = -cx;                                                \
	    buf[2 * bufsize] = cx;                                       \
	}

/* *********************************************************** */

#define	ROTATED_ELLIPSE_PHASE3                                            \
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
	    C * _2fcy1 * _2fcy1 * .25f - D;                               \
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
		d += dinside;                                             \
		dinside += d3e_e;                                         \
		doutside += d3se_e;                                       \
		gradj += grad3e_j;                                        \
		cx++;                                                     \
	    } else {                                                      \
	                                                                  \
/*                                                                        \
 * midpoint is outside - go south east                                    \
 */                                                                       \
		d += doutside;                                            \
		dinside += d3e_se;                                        \
		doutside += d3se_se;                                      \
		gradj += grad3se_j;                                       \
		buf[bufsize - cy] = -cx;                                  \
		cy--;                                                     \
		cx++;                                                     \
	    }                                                             \
	}

/* *********************************************************** */

#define	ROTATED_ELLIPSE_PHASE4                                           \
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
	d = A * _2fcx1 * _2fcx1 * .25f + B * _2fcx1 * fcy1 +             \
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
	buf[bufsize - cy] = -cx;                                         \
	while (cy > maxxy) {                                             \
	                                                                 \
/*                                                                       \
 * midpoint is inside - go south - east                                  \
 */                                                                      \
	    if (d < 0.0f || gradi < 0.0f) {                              \
		d += dinside;                                            \
		dinside += d4se_se;                                      \
		doutside += d4s_se;                                      \
		gradi += grad4se_i;                                      \
		cx++;                                                    \
		cy--;                                                    \
	    } else {                                                     \
	                                                                 \
/*                                                                       \
 * midpoint is outside - go south                                        \
 */                                                                      \
		d += doutside;                                           \
		dinside += d4se_s;                                       \
		doutside += d4s_s;                                       \
		gradi += grad4s_i;                                       \
		cy--;                                                    \
	    }                                                            \
	    buf[bufsize - cy] = -cx;                                     \
	}

/* *********************************************************** */

#define	SIMPLE_ELLIPSE_PHASE1                                       \
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
	d = (b2 - a2 * b + (a2 * .25f));                            \
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
	    } else {                                                \
	                                                            \
/*                                                                  \
 * midpoint is outside - go south - east                            \
 */                                                                 \
		d += doutside;                                      \
		dinside += d1e_se;                                  \
		doutside += d1se_se;                                \
		buf[bufsize - cy] = -cx;                            \
		buf[bufsize + cy] = -cx;                            \
		cx++;                                               \
		cy--;                                               \
		gradj += grad1_j;                                   \
	    }                                                       \
	}

/* *********************************************************** */

#define	SIMPLE_ELLIPSE_PHASE2                                          \
	cx1 = cx + 1;                                                  \
	cy1 = cy - 1;                                                  \
	_2cx = cx * 2;                                                 \
	_2cy = cy * 2;                                                 \
	cx21 = cx1 * 2;                                                \
	cy21 = cy1 * 2;                                                \
	_2cx1 = _2cx + 1;                                              \
	_2cy3 = _2cy - 3;                                              \
/*                                                                     \
 * d( x, y ) = delta F( x, y )                                         \
 */                                                                    \
	d = ((b2 * _2cx1 * _2cx1 - _4a2b2) * .25f) + a2 * cy1 * cy1;   \
/*                                                                     \
 * dinside( x, y ) - delta d( x, y ) with south - east direction       \
 */                                                                    \
	dinside = b2 * cx21 - a2 * _2cy3;                              \
/*                                                                     \
 * doutside( x, y ) - delta d( x, y ) with south direction             \
 */                                                                    \
	doutside = -a2 * _2cy3;                                        \
/*                                                                     \
 * gradj( x, y ) = j coordinate of grad F( x, y )                      \
 */                                                                    \
	gradi = b2 * _2cx1;                                            \
	while (cy > 0) {                                               \
	                                                               \
/*                                                                     \
 * midpoint is inside - go south - east                                \
 */                                                                    \
	    buf[bufsize - cy] = -cx;                                   \
	    buf[bufsize + cy] = -cx;                                   \
	    if (d < 0 || gradi <= 0) {                                 \
		d += dinside;                                          \
		dinside += d2se_se;                                    \
		doutside += d2s_se;                                    \
		gradi += grad2_i;                                      \
		cx++;                                                  \
		cy--;                                                  \
	    } else {                                                   \
	                                                               \
/*                                                                     \
 * midpoint is outside - go south                                      \
 */                                                                    \
		d += doutside;                                         \
		dinside += d2se_s;                                     \
		doutside += d2s_s;                                     \
		cy--;                                                  \
	    }                                                          \
	}                                                              \
	                                                               \
	buf[bufsize - cy] = -cx

/* *********************************************************** */

mlib_status
__mlib_GraphicsFillEllipse_8(
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
	mlib_u8 *line;
	mlib_f32 cosfi, sinfi, cosfi2, sinfi2;
	mlib_s32 doclip = 0;
	mlib_s32 left, right, bottom, top, length;
	mlib_s32 cx, cy;
	mlib_f32 a2, b2;
	mlib_s32 zeroangle = 0;
	mlib_s32 *buf, bufsize, ibuf;

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

		return __mlib_GraphicsDrawLine_8(buffer, x + x1, y + y1, x + x2,
		    y + y2, c);
	}

	if (a == b)
		return (__mlib_GraphicsFillCircle_8(buffer, x, y, a, c));

	length = max(a, b);

	if (mlib_fabs(sinfi * length) < 0.5f)
		zeroangle = 1;
	else if (mlib_fabs(cosfi * length) < 0.5f) {
		mlib_s32 tmp = b;

		b = a;
		a = tmp;
		zeroangle = 1;
	}

	a2 = a * a;
	b2 = b * b;

	if (!zeroangle) {
		mlib_f32 A, B, C, D, A2, B2, C2, _2A, _2B, _2C, _4B, s, z1, z2;
		mlib_f32 fcx, fcy, _2fcx, _2fcy, fcx1, fcy1, fcx21, fcy21,
		    _2fcx1, _2fcy1, _2fcx3, _2fcy3;
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
		mlib_s32 minxx, minxy, maxyx, maxyy, maxxx, maxxy, minyx, minyy;

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
		minxx = -C * s - 0.5f;
		minxy = B * s + ((B > 0.0f) ? 0.5f : -0.5f);
		maxxx = -minxx;
		maxxy = -minxy;
		s = mlib_sqrtf(D / (A2 * C - B2 * A));
		minyy = -A * s - 0.5f;
		minyx = B * s + ((B > 0.0f) ? 0.5f : -0.5f);

		maxyy = -minyy;
		maxyx = -minyx;

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

		bufsize = -minyy;

		if (!(buf =
		    (mlib_s32 *)__mlib_malloc(sizeof (mlib_s32) * (bufsize * 2 +
		    1))))
			return (MLIB_FAILURE);

		cx = minxx;
		cy = minxy;

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

		ROTATED_ELLIPSE_PHASE1;
		ROTATED_ELLIPSE_PHASE2;
		ROTATED_ELLIPSE_PHASE3;
		ROTATED_ELLIPSE_PHASE4;
	} else {
/* Simple algorithm that fills a simple not rotated ellipse */
		mlib_s32 _2a2, _2b2;
		mlib_f32 _4a2b2;
		mlib_s32 cx1, cy1, _2cx, _2cy, _2cx1, _2cy1, _2cx3, _2cy3, cx21,
		    cy21;
		mlib_s32 d, dinside, doutside;
		mlib_s32 gradi, gradj;
		mlib_s32 d1e_e, d1e_se, d1se_e, d1se_se;
		mlib_s32 d2s_s, d2s_se, d2se_s, d2se_se;
		mlib_s32 grad1_j, grad2_i;
		mlib_s32 gradstop1;

		left = x - a;
		right = x + a;
		top = y + b;
		bottom = y - b;

		if (right < 0 || left >= width || top < 0 || bottom >= height)
			return (MLIB_SUCCESS);

		if (left < 0 || right >= width || bottom < 0 || top >= height)
			doclip = 1;

		bufsize = b;

		if (!(buf =
		    (mlib_s32 *)__mlib_malloc(sizeof (mlib_s32) * (b * 2 + 1))))
			return (MLIB_FAILURE);

		_2a2 = a2 * 2;
		_2b2 = b2 * 2;
		_4a2b2 = a2 * b2 * 4;

		gradstop1 = a2 / mlib_sqrtf(a2 + b2) + 0.5f;

		cx = 0;
		cy = b;

		d1se_e = d1e_se = d1e_e = _2b2;
		d2se_se = d1se_se = _2b2 + _2a2;
		d2se_s = d2s_se = d2s_s = _2a2;

		grad1_j = -_2a2;
		grad2_i = _2b2;

		SIMPLE_ELLIPSE_PHASE1;
		SIMPLE_ELLIPSE_PHASE2;
	}

	{
		mlib_d64 dcolor;
		mlib_s32 color;

		line = data + (y - bufsize) * stride;
		color = c & 0xff;
		color |= (color << 8);
		color |= (color << 16);
		DOUBLE_FROM_INT(dcolor, color);

		if (!doclip) {
			for (ibuf = 0; 2 * bufsize - ibuf >= 0; ++ibuf) {
				mlib_s32 beg = x + buf[2 * bufsize - ibuf];
				mlib_s32 end = x - buf[ibuf];

				MLIB_FILL_ROW_8(line, beg, end, c, dcolor);
				line += stride;
			}
		} else {
			mlib_s32 yb = (bufsize - y > 0) ? bufsize - y : 0;
			mlib_s32 ye =
			    (bufsize >
			    height - 1 - y) ? bufsize + (height - 1 -
			    y) : bufsize * 2;

			line += yb * stride;
			for (ibuf = yb; ibuf <= ye; ++ibuf) {
				mlib_s32 end = x - buf[ibuf];
				mlib_s32 beg = x + buf[2 * bufsize - ibuf];
				mlib_s32 mask = ((width - 1) - end) >> 31;

				beg &= ~(beg >> 31);
				end = (end & ~mask) | ((width - 1) & mask);
				MLIB_FILL_ROW_8(line, beg, end, c, dcolor);
				line += stride;
			}
		}
	}

	__mlib_free(buf);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsFillEllipse_32(
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
	mlib_s32 *line;
	mlib_f32 cosfi, sinfi, cosfi2, sinfi2;
	mlib_s32 doclip = 0;
	mlib_s32 left, right, bottom, top, length;
	mlib_s32 cx, cy;
	mlib_f32 a2, b2;
	mlib_s32 zeroangle = 0;
	mlib_s32 *buf, bufsize, ibuf;

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

		return __mlib_GraphicsDrawLine_32(buffer, x + x1, y + y1,
		    x + x2, y + y2, c);
	}

	if (a == b)
		return (__mlib_GraphicsFillCircle_32(buffer, x, y, a, c));

	length = max(a, b);

	if (mlib_fabs(sinfi * length) < 0.5f)
		zeroangle = 1;
	else if (mlib_fabs(cosfi * length) < 0.5f) {
		mlib_s32 tmp = b;

		b = a;
		a = tmp;
		zeroangle = 1;
	}

	a2 = a * a;
	b2 = b * b;

	if (!zeroangle) {
		mlib_f32 A, B, C, D, A2, B2, C2, _2A, _2B, _2C, _4B, s, z1, z2;
		mlib_f32 fcx, fcy, _2fcx, _2fcy, fcx1, fcy1, fcx21, fcy21,
		    _2fcx1, _2fcy1, _2fcx3, _2fcy3;
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
		mlib_s32 minxx, minxy, maxyx, maxyy, maxxx, maxxy, minyx, minyy;

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
		minxx = -C * s - 0.5f;
		minxy = B * s + ((B > 0.0f) ? 0.5f : -0.5f);
		maxxx = -minxx;
		maxxy = -minxy;
		s = mlib_sqrtf(D / (A2 * C - B2 * A));
		minyy = -A * s - 0.5f;
		minyx = B * s + ((B > 0.0f) ? 0.5f : -0.5f);

		maxyy = -minyy;
		maxyx = -minyx;

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

		bufsize = ((minyy > 0) ? minyy : -minyy);

		if (!(buf =
		    (mlib_s32 *)__mlib_malloc(sizeof (mlib_s32) * (bufsize * 2 +
		    1))))
			return (MLIB_FAILURE);

		cx = minxx;
		cy = minxy;

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

		ROTATED_ELLIPSE_PHASE1;
		ROTATED_ELLIPSE_PHASE2;
		ROTATED_ELLIPSE_PHASE3;
		ROTATED_ELLIPSE_PHASE4;
	} else {
/* Simple algorithm that fills a simple not rotated ellipse */
		mlib_s32 _2a2, _2b2;
		mlib_f32 _4a2b2;
		mlib_s32 cx1, cy1, _2cx, _2cy, _2cx1, _2cy1, _2cx3, _2cy3, cx21,
		    cy21;
		mlib_s32 d, dinside, doutside;
		mlib_s32 gradi, gradj;
		mlib_s32 d1e_e, d1e_se, d1se_e, d1se_se;
		mlib_s32 d2s_s, d2s_se, d2se_s, d2se_se;
		mlib_s32 grad1_j, grad2_i;
		mlib_s32 gradstop1;

		left = x - a;
		right = x + a;
		top = y + b;
		bottom = y - b;

		if (right < 0 || left >= width || top < 0 || bottom >= height)
			return (MLIB_SUCCESS);

		if (left < 0 || right >= width || bottom < 0 || top >= height)
			doclip = 1;

		bufsize = b;

		if (!(buf =
		    (mlib_s32 *)__mlib_malloc(sizeof (mlib_s32) * (b * 2 + 1))))
			return (MLIB_FAILURE);

		_2a2 = a2 * 2;
		_2b2 = b2 * 2;
		_4a2b2 = a2 * b2 * 4;

		gradstop1 = a2 / mlib_sqrtf(a2 + b2) + 0.5f;

		cx = 0;
		cy = b;

		d1se_e = d1e_se = d1e_e = _2b2;
		d2se_se = d1se_se = _2b2 + _2a2;
		d2se_s = d2s_se = d2s_s = _2a2;

		grad1_j = -_2a2;
		grad2_i = _2b2;

		SIMPLE_ELLIPSE_PHASE1;
		SIMPLE_ELLIPSE_PHASE2;
	}

	{
		mlib_d64 dcolor;

		line = data + (y - bufsize) * stride;
		DOUBLE_FROM_INT(dcolor, c);

		if (!doclip) {
			for (ibuf = 0; 2 * bufsize - ibuf >= 0; ++ibuf) {
				mlib_s32 beg = x + buf[2 * bufsize - ibuf];
				mlib_s32 end = x - buf[ibuf];

				MLIB_FILL_ROW_32(line, beg, end, c, dcolor);
				line += stride;
			}
		} else {
			mlib_s32 yb = (bufsize - y > 0) ? bufsize - y : 0;
			mlib_s32 ye =
			    (bufsize >
			    height - 1 - y) ? bufsize + (height - 1 -
			    y) : bufsize * 2;

			line += yb * stride;
			for (ibuf = yb; ibuf <= ye; ++ibuf) {
				mlib_s32 end = x - buf[ibuf];
				mlib_s32 beg = x + buf[2 * bufsize - ibuf];
				mlib_s32 mask = ((width - 1) - end) >> 31;

				beg &= ~(beg >> 31);
				end = (end & ~mask) | ((width - 1) & mask);
				MLIB_FILL_ROW_32(line, beg, end, c, dcolor);
				line += stride;
			}
		}
	}

	__mlib_free(buf);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
