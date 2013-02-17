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

#pragma ident	"@(#)mlib_c_GraphicsSpamLineSet_ABGZ.c	9.2	07/10/09 SMI"

/*
 *  FUNCTIONS
 *    mlib_GraphicsSpamLineSet_ABGZ_8  - line drawing function for mlib_u8 image
 *    mlib_GraphicsSpamLineSet_ABGZ_32 - line drawing function
 *							for mlib_s32 image
 *
 *  SYNOPSIS
 *    void  mlib_GraphicsSpamLineSet_ABGZ_8
 *                                    (mlib_u8  **rtable,
 *                                     mlib_u8  **zrtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 zstride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *				       mlib_s32 alpha,
 *                                     mlib_s32 width,
 *                                     mlib_s32 height);
 *    void  mlib_GraphicsSpamLineSet_ABGZ_32
 *                                    (mlib_u8  **rtable,
 *                                     mlib_u8  **zrtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 zstride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *				       mlib_s32 alpha,
 *                                     mlib_s32 width,
 *                                     mlib_s32 height);
 *
 *
 *  ARGUMENTS
 *    rtable      Pointer to array of starts of row, rtable[-1][-1] is buffer
 *                  for unwanted stores
 *    zrtable     Pointer to array of starts of row of zbuffer, zrtable[-1][-1]
 *                  is buffer for unwanted stores
 *    stride      Stride of the image
 *    zstride     Stride of the zbuffer image
 *    coords      Array of values sets using in *Spam* functions:
 *                  coords[MLIB_GDS_STARTX + i * MLIB_GDS_NEXT]      holds X
 *                  coords[MLIB_GDS_STARTY + i * MLIB_GDS_NEXT]      holds Y
 *                  coords[MLIB_GDS_LENGTHLINES + i * MLIB_GDS_NEXT] holds LL
 *                  coords[MLIB_GDS_STEPLINES + i * MLIB_GDS_NEXT]   holds SL
 *                  if |dx| > |dy| then
 *                    X  = x0, Y = ~(MLIB_HALF + (y0 << MLIB_SHIFT))
 *                    LL = |dx|, SL = (mlib_s32) (32768. * dy / LL)
 *                  else
 *                    X = ~(MLIB_HALF + (x0 << MLIB_SHIFT)), Y = y0
 *                    LL = |dy|, SL = (mlib_s32) (32768. * dx / LL)
 *                  endif
 *                , so we could understand in *Spam* functions what is
 *                bigger |dx| or |dy| by negative value of opposite starting
 *                value. Note we always step by +1 by main axis, never -1.
 *    coordsend   for(; coords < coordsend; coords += MLIB_GDS_NEXT)
 *    width       width = Image.Width - 2, is using to discover to clip
 *                neighbour in *Spam_A* functions
 *    height      height = Image.Height - 2, is using to discover to
 *                clip neighbour in *Spam_A* functions
 *    alpha	  Alpha blending value.
 */

#include <mlib_graphics.h>
#include <mlib_GraphicsDrawSet.h>
#include <mlib_ImageDivTables.h>

/* *********************************************************** */

#ifdef _LP64

#define	MLIB_GET_POINTER(RT, T)                                            \
	* (mlib_u8 **)((mlib_u8 *)RT + (((T) >> (MLIB_SHIFT - 3)) & ~7))

#define	MLIB_GET_POINTER2(RT, T, ZZ)                             \
	* (mlib_u8 **)((mlib_u8 *)RT + ((((T) >> (MLIB_SHIFT -   \
		3)) | ZZ) & ~7))

#define	MLIB_POINTER_COEF	8
#define	MLIB_POINTER_MUL(X)	2 * (X)

#else /* _LP64 */

#define	MLIB_GET_POINTER(RT, T)                                            \
	* (mlib_u8 **)((mlib_u8 *)RT + (((T) >> (MLIB_SHIFT - 2)) & ~3))

#define	MLIB_GET_POINTER2(RT, T, ZZ)                             \
	* (mlib_u8 **)((mlib_u8 *)RT + ((((T) >> (MLIB_SHIFT -   \
		2)) | ZZ) & ~3))

#define	MLIB_POINTER_COEF	4
#define	MLIB_POINTER_MUL(X)	(X)
#endif /* _LP64 */

/* *********************************************************** */

static void mlib_GraphicsSpamLineSet_ABGZ_8_InnX(
	mlib_u8 **rtable,
	mlib_u8 **zrtable,
	mlib_s32 *coords,
	mlib_s32 height,
	mlib_s32 a);

static void mlib_GraphicsSpamLineSet_ABGZ_8_InnHor(
	mlib_u8 **rtable,
	mlib_u8 **zrtable,
	mlib_s32 *coords,
	mlib_s32 height,
	mlib_s32 a);

static void mlib_GraphicsSpamLineSet_ABGZ_8_InnY(
	mlib_u8 **rtable,
	mlib_u8 **zrtable,
	mlib_s32 *coords,
	mlib_s32 width,
	mlib_s32 a);

static void mlib_GraphicsSpamLineSet_ABGZ_8_InnVer(
	mlib_u8 **rtable,
	mlib_u8 **zrtable,
	mlib_s32 *coords,
	mlib_s32 width,
	mlib_s32 a);

static void mlib_GraphicsSpamLineSet_ABGZ_32_InnX(
	mlib_u8 **rtable,
	mlib_u8 **zrtable,
	mlib_s32 *coords,
	mlib_s32 height,
	mlib_s32 a);

static void mlib_GraphicsSpamLineSet_ABGZ_32_InnHor(
	mlib_u8 **rtable,
	mlib_u8 **zrtable,
	mlib_s32 *coords,
	mlib_s32 height,
	mlib_s32 a);

static void mlib_GraphicsSpamLineSet_ABGZ_32_InnY(
	mlib_u8 **rtable,
	mlib_u8 **zrtable,
	mlib_s32 *coords,
	mlib_s32 width,
	mlib_s32 a);

static void mlib_GraphicsSpamLineSet_ABGZ_32_InnVer(
	mlib_u8 **rtable,
	mlib_u8 **zrtable,
	mlib_s32 *coords,
	mlib_s32 width,
	mlib_s32 a);

/* *********************************************************** */

#define	MLIB_BLENDING_8(s, c)                                   \
	s = (mlib_U82D64[s] * A0) + (mlib_U82D64[c] * A1)

/* *********************************************************** */

#define	MLIB_BLENDING_8_PURE(s, c)                              \
	(mlib_U82D64[s] * A0) + (mlib_U82D64[c] * A1)

/* *********************************************************** */

void
mlib_GraphicsSpamLineSet_ABGZ_8_InnX(
	mlib_u8 **rtable,
	mlib_u8 **zrtable,
	mlib_s32 *coords,
	mlib_s32 height,
	mlib_s32 a)
{
	mlib_u8 *atable, *udata, *data, *ddata, *udivt, *divt, *ddivt;
	mlib_s16 *zdata;
	mlib_s32 j, x, y, z, n, t, dt, dz, stz, offset, zz, xz, ux, dx;
	mlib_s32 bg, ubg, dbg, fg, ufg, dfg;
	mlib_s32 c, tc, dc;
	mlib_d64 A0, A1;

	A1 = (a & 0xFF) / 255.;
	A0 = 1. - A1;

	n = coords[MLIB_GDS_LENGTHLINES];
	t = ~coords[MLIB_GDS_STARTY];
	x = coords[MLIB_GDS_STARTX];
	dt = coords[MLIB_GDS_STEPLINES];
	z = ~coords[MLIB_GDS_STARTZ];
	dz = coords[MLIB_GDS_STEPZ];
	tc = ~coords[MLIB_GDS_STARTC];
	dc = coords[MLIB_GDS_STEPC];
	zz = dt >> 31;
	zz = (dt ^ zz) - zz;
	atable = mlib_GraphicsAliasTab_u8 +
		512 * mlib_GraphicsSqrtTab_u8[zz >> (MLIB_SHIFT - 7)];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j <= n; j++, x++) {
		stz = z >> MLIB_SHIFT;
		z += dz;
		c = tc >> MLIB_SHIFT;
		tc += dc;
		zdata = (mlib_s16 *)(MLIB_GET_POINTER(zrtable, t));
		zz = (zdata[x] - stz) >> 31;
		offset = (t >> (MLIB_SHIFT - 8)) & 0xFF;
		divt = mlib_GraphicsDivTab_u8 + (atable[offset + 256] << 8);
		udivt = mlib_GraphicsDivTab_u8 + (atable[offset ^ 0xFF] << 8);
		ddivt = mlib_GraphicsDivTab_u8 + (atable[offset] << 8);
		y = t >> MLIB_SHIFT;
		t += dt;
		xz = x | zz;
		data = rtable[y | zz];
		zdata = (mlib_s16 *)(zrtable[y | zz]);
		udata = rtable[(y - 1) | zz];
		ddata = rtable[(y + 1) | zz];
		ux = ((y - 1) >> 31) | xz;
		dx = ((height - y) >> 31) | xz;
		bg = data[xz];
		ubg = udata[ux];
		dbg = ddata[dx];
		zdata[xz] = stz;
		fg = bg + (divt[c] - divt[bg]);
		ufg = ubg + (udivt[c] - udivt[ubg]);
		dfg = dbg + (ddivt[c] - ddivt[dbg]);
		data[xz] = MLIB_BLENDING_8_PURE(bg, fg);
		udata[ux] = MLIB_BLENDING_8_PURE(ubg, ufg);
		ddata[dx] = MLIB_BLENDING_8_PURE(dbg, dfg);
	}
}

/* *********************************************************** */

void
mlib_GraphicsSpamLineSet_ABGZ_8_InnHor(
	mlib_u8 **rtable,
	mlib_u8 **zrtable,
	mlib_s32 *coords,
	mlib_s32 height,
	mlib_s32 a)
{
	mlib_u8 *udata, *data, *ddata, *divt;
	mlib_s16 *zdata, *zdata1;
	mlib_s32 j, x, y, z, n, dz, stz, zz, xz, ux, dx;
	mlib_s32 ualpha, alpha, dalpha, bg, ubg, dbg, ufg, dfg;
	mlib_s32 c, tc, dc;
	mlib_d64 A0, A1;

	A1 = (a & 0xFF) / 255.;
	A0 = 1. - A1;

	n = coords[MLIB_GDS_LENGTHLINES];
	y = (~coords[MLIB_GDS_STARTY]) >> MLIB_SHIFT;
	x = coords[MLIB_GDS_STARTX];
	z = ~coords[MLIB_GDS_STARTZ];
	dz = coords[MLIB_GDS_STEPZ];
	tc = ~coords[MLIB_GDS_STARTC];
	dc = coords[MLIB_GDS_STEPC];
	zdata = (mlib_s16 *)(zrtable[y]);
	divt = mlib_GraphicsDivTab_u8 + (MLIB_BG_COEFF << 8);
	ualpha = (y - 1) >> 31;
	dalpha = (height - y) >> 31;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j <= n; j++, x++) {
		stz = z >> MLIB_SHIFT;
		z += dz;
		c = tc >> MLIB_SHIFT;
		tc += dc;
		zz = (zdata[x] - stz) >> 31;
		ux = ualpha | zz;
		dx = dalpha | zz;
		data = rtable[y | zz];
		zdata1 = (mlib_s16 *)(zrtable[y | zz]);
		udata = rtable[(y - 1) | ux];
		ddata = rtable[(y + 1) | dx];
		xz = x | zz;
		ux = x | ux;
		dx = x | dx;
		alpha = divt[c];
		bg = data[xz];
		ubg = udata[ux];
		dbg = ddata[dx];
		zdata1[xz] = stz;
		ufg = ubg + alpha - divt[ubg];
		dfg = dbg + alpha - divt[dbg];
		data[xz] = MLIB_BLENDING_8_PURE(bg, c);
		udata[ux] = MLIB_BLENDING_8_PURE(ubg, ufg);
		ddata[dx] = MLIB_BLENDING_8_PURE(dbg, dfg);
	}
}

/* *********************************************************** */

void
mlib_GraphicsSpamLineSet_ABGZ_8_InnY(
	mlib_u8 **rtable,
	mlib_u8 **zrtable,
	mlib_s32 *coords,
	mlib_s32 width,
	mlib_s32 a)
{
	mlib_u8 *atable, *udata, *data, *ddata, *udivt, *divt, *ddivt;
	mlib_s16 *zdata;
	mlib_s32 j, x, y, z, n, t, dt, dz, stz, offset, zz, xz, ux, dx;
	mlib_s32 c, tc, dc;
	mlib_s32 bg, ubg, dbg, fg, ufg, dfg;
	mlib_d64 A0, A1;

	A1 = (a & 0xFF) / 255.;
	A0 = 1. - A1;

	n = coords[MLIB_GDS_LENGTHLINES];
	y = coords[MLIB_GDS_STARTY];
	t = ~coords[MLIB_GDS_STARTX];
	dt = coords[MLIB_GDS_STEPLINES];
	z = ~coords[MLIB_GDS_STARTZ];
	dz = coords[MLIB_GDS_STEPZ];
	tc = ~coords[MLIB_GDS_STARTC];
	dc = coords[MLIB_GDS_STEPC];
	zz = dt >> 31;
	zz = (dt ^ zz) - zz;
	atable = mlib_GraphicsAliasTab_u8 +
		512 * mlib_GraphicsSqrtTab_u8[zz >> (MLIB_SHIFT - 7)];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j <= n; j++, y++) {
		stz = z >> MLIB_SHIFT;
		z += dz;
		x = t >> MLIB_SHIFT;
		zdata = (mlib_s16 *)(zrtable[y]);
		zz = (zdata[x] - stz) >> 31;
		offset = (t >> (MLIB_SHIFT - 8)) & 0xFF;
		divt = mlib_GraphicsDivTab_u8 + (atable[offset + 256] << 8);
		udivt = mlib_GraphicsDivTab_u8 + (atable[offset ^ 0xFF] << 8);
		ddivt = mlib_GraphicsDivTab_u8 + (atable[offset] << 8);
		t += dt;
		xz = x | zz;
		ux = (x - 1) | zz;
		dx = ((width - x) >> 31) | zz;
		data = rtable[y | zz];
		zdata = (mlib_s16 *)(zrtable[y | zz]);
		udata = rtable[y | (ux >> 31)];
		ddata = rtable[y | dx];
		dx = (x + 1) | dx;
		c = tc >> MLIB_SHIFT;
		tc += dc;
		bg = data[xz];
		ubg = udata[ux];
		dbg = ddata[dx];
		zdata[xz] = stz;
		fg = bg + divt[c] - divt[bg];
		ufg = ubg + udivt[c] - udivt[ubg];
		dfg = dbg + ddivt[c] - ddivt[dbg];
		data[xz] = MLIB_BLENDING_8_PURE(bg, fg);
		udata[ux] = MLIB_BLENDING_8_PURE(ubg, ufg);
		ddata[dx] = MLIB_BLENDING_8_PURE(dbg, dfg);
	}
}

/* *********************************************************** */

void
mlib_GraphicsSpamLineSet_ABGZ_8_InnVer(
	mlib_u8 **rtable,
	mlib_u8 **zrtable,
	mlib_s32 *coords,
	mlib_s32 width,
	mlib_s32 a)
{
	mlib_u8 *udata, *data, *ddata, *divt;
	mlib_s16 *zdata;
	mlib_s32 x, y, z, n, dt, dz, stz, zz, xz, ux, dx;
	mlib_s32 alpha, ubg, dbg, ufg, dfg;
	mlib_d64 A0, A1;
	mlib_s32 c, tc, dc;

	A1 = (a & 0xFF) / 255.;
	A0 = 1. - A1;

	n = coords[MLIB_GDS_LENGTHLINES];
	y = coords[MLIB_GDS_STARTY];
	x = (~coords[MLIB_GDS_STARTX]) >> MLIB_SHIFT;
	dt = coords[MLIB_GDS_STEPLINES];
	z = ~coords[MLIB_GDS_STARTZ];
	dz = coords[MLIB_GDS_STEPZ];
	tc = ~coords[MLIB_GDS_STARTC];
	dc = coords[MLIB_GDS_STEPC];
	alpha = (dt) ? MLIB_DG_COEFF : MLIB_BG_COEFF;
	dt = dt >> MLIB_SHIFT;
	divt = mlib_GraphicsDivTab_u8 + (alpha << 8);
	n += y;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; y <= n; x += dt) {
		stz = z >> MLIB_SHIFT;
		z += dz;
		c = tc >> MLIB_SHIFT;
		tc += dc;
		zdata = (mlib_s16 *)(zrtable[y]);
		zz = (zdata[x] - stz) >> 31;
		xz = x | zz;
		ux = (x - 1) | zz;
		dx = ((width - x) >> 31) | zz;
		data = rtable[y | zz];
		zdata = (mlib_s16 *)(zrtable[y | zz]);
		udata = rtable[y | (ux >> 31)];
		ddata = rtable[y | dx];
		dx = (x + 1) | dx;
		alpha = divt[c];
		zdata[xz] = stz;
		MLIB_BLENDING_8(data[xz], c);
		ubg = udata[ux];
		dbg = ddata[dx];
		ufg = ubg + alpha - divt[ubg];
		dfg = dbg + alpha - divt[dbg];
		udata[ux] = MLIB_BLENDING_8_PURE(ubg, ufg);
		ddata[dx] = MLIB_BLENDING_8_PURE(dbg, dfg);
		y++;
	}
}

/* *********************************************************** */

void
mlib_GraphicsSpamLineSet_ABGZ_8(
	mlib_u8 **rtable,
	mlib_u8 **zrtable,
	mlib_s32 stride,
	mlib_s32 zstride,
	mlib_s32 *coords,
	mlib_s32 *coordsend,
	mlib_s32 a,
	mlib_s32 width,
	mlib_s32 height)
{
	mlib_s32 n, y, dt;

	n = stride;
	y = zstride;

	while (coords < coordsend) {
		n = coords[MLIB_GDS_LENGTHLINES];
		y = coords[MLIB_GDS_STARTY];
		dt = coords[MLIB_GDS_STEPLINES];

		if (n >= 0) {
			if (y < 0) {
				if (dt)
					mlib_GraphicsSpamLineSet_ABGZ_8_InnX
						(rtable, zrtable, coords,
						height, a);
				else
					mlib_GraphicsSpamLineSet_ABGZ_8_InnHor
						(rtable, zrtable, coords,
						height, a);
			} else {
				if (dt & (MLIB_ONE - 1))
					mlib_GraphicsSpamLineSet_ABGZ_8_InnY
						(rtable, zrtable, coords,
						width, a);
				else
					mlib_GraphicsSpamLineSet_ABGZ_8_InnVer
						(rtable, zrtable, coords,
						width, a);
			}
		}

		coords += MLIB_GDS_NEXT_GZ;
	}
}

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN

#define	MLIB_ALIAS32(DATA, OFF, OFFSET)                             \
	{                                                           \
	    mlib_u8 *_dtable = mlib_GraphicsDivTab_u8 + (OFFSET);   \
	    mlib_u8 *_data = DATA + (OFF);                          \
	    mlib_s32 fg1, fg2, fg3;                                 \
	    mlib_s32 bg1, bg2, bg3;                                 \
	                                                            \
	    _data[3] = 0xFF;                                        \
	    bg1 = _data[2];                                         \
	    bg2 = _data[1];                                         \
	    fg1 = bg1 + (_dtable[c2] - _dtable[bg1]);               \
	    _data[2] = MLIB_BLENDING_8_PURE(bg1, fg1);              \
	    bg3 = _data[0];                                         \
	    fg2 = bg2 + (_dtable[c1] - _dtable[bg2]);               \
	    fg3 = bg3 + (_dtable[c0] - _dtable[bg3]);               \
	    _data[1] = MLIB_BLENDING_8_PURE(bg2, fg2);              \
	    _data[0] = MLIB_BLENDING_8_PURE(bg3, fg3);              \
	}

#define	MLIB_SIMPLE_ALIAS32(DATA, OFF, DIVT)                    \
	{                                                       \
	    mlib_u8 *_data = DATA + (OFF);                      \
	    mlib_s32 bg1, bg2, bg3;                             \
	    mlib_s32 fg1, fg2, fg3;                             \
	                                                        \
	    _data[3] = 0xFF;                                    \
	    bg1 = _data[2];                                     \
	    bg2 = _data[1];                                     \
	    bg3 = _data[0];                                     \
	    fg1 = bg1 + (DIVT[c2] - DIVT[bg1]);                 \
	    fg2 = bg2 + (DIVT[c1] - DIVT[bg2]);                 \
	    fg3 = bg3 + (DIVT[c0] - DIVT[bg3]);                 \
	    _data[2] = MLIB_BLENDING_8_PURE(bg1, fg1);          \
	    _data[1] = MLIB_BLENDING_8_PURE(bg2, fg2);          \
	    _data[0] = MLIB_BLENDING_8_PURE(bg3, fg3);          \
	}

#define	MLIB_PUT_ABGZ_32(DATA, OFF)                                \
	{                                                       \
	    mlib_u8 *_data = DATA + (OFF);                      \
	                                                        \
	    _data[3] = 0xFF;                                    \
	    MLIB_BLENDING_8(_data[2], c2);                      \
	    MLIB_BLENDING_8(_data[1], c1);                      \
	    MLIB_BLENDING_8(_data[0], c0);                      \
	}

#else /* _LITTLE_ENDIAN */

#define	MLIB_ALIAS32(DATA, OFF, OFFSET)                             \
	{                                                           \
	    mlib_u8 *_dtable = mlib_GraphicsDivTab_u8 + (OFFSET);   \
	    mlib_u8 *_data = DATA + (OFF);                          \
	    mlib_s32 fg1, fg2, fg3;                                 \
	    mlib_s32 bg1, bg2, bg3;                                 \
	                                                            \
	    _data[0] = 0xFF;                                        \
	    bg1 = _data[1];                                         \
	    bg2 = _data[2];                                         \
	    fg1 = bg1 + (_dtable[c2] - _dtable[bg1]);               \
	    _data[1] = MLIB_BLENDING_8_PURE(bg1, fg1);              \
	    bg3 = _data[3];                                         \
	    fg2 = bg2 + (_dtable[c1] - _dtable[bg2]);               \
	    fg3 = bg3 + (_dtable[c0] - _dtable[bg3]);               \
	    _data[2] = MLIB_BLENDING_8_PURE(bg2, fg2);              \
	    _data[3] = MLIB_BLENDING_8_PURE(bg3, fg3);              \
	}

#define	MLIB_SIMPLE_ALIAS32(DATA, OFF, DIVT)                    \
	{                                                       \
	    mlib_u8 *_data = DATA + (OFF);                      \
	    mlib_s32 bg1, bg2, bg3;                             \
	    mlib_s32 fg1, fg2, fg3;                             \
	                                                        \
	    _data[0] = 0xFF;                                    \
	    bg1 = _data[1];                                     \
	    bg2 = _data[2];                                     \
	    bg3 = _data[3];                                     \
	    fg1 = bg1 + (DIVT[c2] - DIVT[bg1]);                 \
	    fg2 = bg2 + (DIVT[c1] - DIVT[bg2]);                 \
	    fg3 = bg3 + (DIVT[c0] - DIVT[bg3]);                 \
	    _data[1] = MLIB_BLENDING_8_PURE(bg1, fg1);          \
	    _data[2] = MLIB_BLENDING_8_PURE(bg2, fg2);          \
	    _data[3] = MLIB_BLENDING_8_PURE(bg3, fg3);          \
	}

#define	MLIB_PUT_ABGZ_32(DATA, OFF)                                \
	{                                                       \
	    mlib_u8 *_data = DATA + (OFF);                      \
	                                                        \
	    _data[0] = 0xFF;                                    \
	    MLIB_BLENDING_8(_data[1], c2);                      \
	    MLIB_BLENDING_8(_data[2], c1);                      \
	    MLIB_BLENDING_8(_data[3], c0);                      \
	}

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

void
mlib_GraphicsSpamLineSet_ABGZ_32_InnX(
	mlib_u8 **rtable,
	mlib_u8 **zrtable,
	mlib_s32 *coords,
	mlib_s32 height,
	mlib_s32 a)
{
	mlib_u8 *atable, *udata, *data, *ddata;
	mlib_s16 *zdata;
	mlib_s32 j, x, y, z, n, t, dt, dz, stz, offset, zz, ux, dx;
	mlib_s32 c0, c1, c2, tc, dc, tc1, dc1, tc2, dc2;
	mlib_d64 A0, A1;

	A1 = (a & 0xFF) / 255.;
	A0 = 1. - A1;

	n = coords[MLIB_GDS_LENGTHLINES];
	t = ~coords[MLIB_GDS_STARTY];
	x = coords[MLIB_GDS_STARTX];
	dt = coords[MLIB_GDS_STEPLINES];
	z = ~coords[MLIB_GDS_STARTZ];
	dz = coords[MLIB_GDS_STEPZ];
	tc2 = ~coords[MLIB_GDS_STARTC2];
	dc2 = coords[MLIB_GDS_STEPC2];
	tc1 = ~coords[MLIB_GDS_STARTC1];
	dc1 = coords[MLIB_GDS_STEPC1];
	tc = ~coords[MLIB_GDS_STARTC];
	dc = coords[MLIB_GDS_STEPC];
	zz = dt >> 31;
	zz = (dt ^ zz) - zz;
	atable = mlib_GraphicsAliasTab_u8 +
		512 * mlib_GraphicsSqrtTab_u8[zz >> (MLIB_SHIFT - 7)];

	for (j = 0; j <= n; j++, x++) {
		stz = z >> MLIB_SHIFT;
		z += dz;
		zdata = (mlib_s16 *)(MLIB_GET_POINTER(zrtable, t));
		zz = zdata[x] - stz;
		c2 = tc2 >> MLIB_SHIFT;
		tc2 += dc2;
		c1 = tc1 >> MLIB_SHIFT;
		tc1 += dc1;
		c0 = tc >> MLIB_SHIFT;
		tc += dc;

		if (zz >= 0) {
			y = t >> MLIB_SHIFT;
			zdata = (mlib_s16 *)(zrtable[y]);
			data = rtable[y];
			udata = rtable[(y - 1)];
			ddata = rtable[(y + 1)];
			ux = ((y - 1) >> 31) | x;
			dx = ((height - y) >> 31) | x;
			offset = (t >> (MLIB_SHIFT - 8)) & 0xFF;
			zdata[x] = stz;
			MLIB_ALIAS32(data, 4 * x, atable[offset + 256] << 8)
			MLIB_ALIAS32(udata, 4 * ux, atable[offset ^ 0xFF] << 8)
			MLIB_ALIAS32(ddata, 4 * dx, atable[offset] << 8)
		}

		t += dt;
	}
}

/* *********************************************************** */

void
mlib_GraphicsSpamLineSet_ABGZ_32_InnHor(
	mlib_u8 **rtable,
	mlib_u8 **zrtable,
	mlib_s32 *coords,
	mlib_s32 height,
	mlib_s32 a)
{
	mlib_u8 *udata, *data, *ddata, *divt;
	mlib_s16 *zdata, *zdata1;
	mlib_s32 j, x, y, z, n, dz, stz, zz, ux, dx, xz;
	mlib_s32 ualpha, dalpha;
	mlib_s32 c0, c1, c2, tc, dc, tc1, dc1, tc2, dc2;
	mlib_d64 A0, A1;

	A1 = (a & 0xFF) / 255.;
	A0 = 1. - A1;

	n = coords[MLIB_GDS_LENGTHLINES];
	y = (~coords[MLIB_GDS_STARTY]) >> MLIB_SHIFT;
	x = coords[MLIB_GDS_STARTX];
	z = ~coords[MLIB_GDS_STARTZ];
	dz = coords[MLIB_GDS_STEPZ];
	tc2 = ~coords[MLIB_GDS_STARTC2];
	dc2 = coords[MLIB_GDS_STEPC2];
	tc1 = ~coords[MLIB_GDS_STARTC1];
	dc1 = coords[MLIB_GDS_STEPC1];
	tc = ~coords[MLIB_GDS_STARTC];
	dc = coords[MLIB_GDS_STEPC];
	zdata = (mlib_s16 *)(zrtable[y]);
	divt = mlib_GraphicsDivTab_u8 + (MLIB_BG_COEFF << 8);
	ualpha = (y - 1) >> 31;
	dalpha = (height - y) >> 31;

	for (j = 0; j <= n; j++, x++) {
		stz = z >> MLIB_SHIFT;
		z += dz;
		zz = (zdata[x] - stz) >> 31;
		ux = ualpha | zz;
		dx = dalpha | zz;
		data = rtable[y | zz];
		zdata1 = (mlib_s16 *)(zrtable[y | zz]);
		udata = rtable[(y - 1) | ux];
		ddata = rtable[(y + 1) | dx];
		xz = x | zz;
		ux = x | ux;
		dx = x | dx;
		c2 = tc2 >> MLIB_SHIFT;
		tc2 += dc2;
		c1 = tc1 >> MLIB_SHIFT;
		tc1 += dc1;
		c0 = tc >> MLIB_SHIFT;
		tc += dc;
		MLIB_SIMPLE_ALIAS32(udata, 4 * ux, divt);
		MLIB_SIMPLE_ALIAS32(ddata, 4 * dx, divt);
		zdata1[xz] = stz;
		MLIB_PUT_ABGZ_32(data, 4 * xz);
	}
}

/* *********************************************************** */

void
mlib_GraphicsSpamLineSet_ABGZ_32_InnY(
	mlib_u8 **rtable,
	mlib_u8 **zrtable,
	mlib_s32 *coords,
	mlib_s32 width,
	mlib_s32 a)
{
	mlib_u8 *atable, *udata, *data, *ddata;
	mlib_s16 *zdata;
	mlib_s32 j, x, y, z, n, t, dt, dz, stz, offset, zz, ux, dx;
	mlib_s32 c0, c1, c2, tc, dc, tc1, dc1, tc2, dc2;
	mlib_d64 A0, A1;

	A1 = (a & 0xFF) / 255.;
	A0 = 1. - A1;

	n = coords[MLIB_GDS_LENGTHLINES];
	y = 4 * coords[MLIB_GDS_STARTY];
	t = ~coords[MLIB_GDS_STARTX];
	dt = coords[MLIB_GDS_STEPLINES];
	z = ~coords[MLIB_GDS_STARTZ];
	dz = coords[MLIB_GDS_STEPZ];
	tc2 = ~coords[MLIB_GDS_STARTC2];
	dc2 = coords[MLIB_GDS_STEPC2];
	tc1 = ~coords[MLIB_GDS_STARTC1];
	dc1 = coords[MLIB_GDS_STEPC1];
	tc = ~coords[MLIB_GDS_STARTC];
	dc = coords[MLIB_GDS_STEPC];
	zz = dt >> 31;
	zz = (dt ^ zz) - zz;
	atable = mlib_GraphicsAliasTab_u8 +
		512 * mlib_GraphicsSqrtTab_u8[zz >> (MLIB_SHIFT - 7)];

	for (j = 0; j <= n; j++, y += 4) {
		stz = z >> MLIB_SHIFT;
		z += dz;
		x = (t >> (MLIB_SHIFT - 2)) & ~3;
		zdata = *(mlib_s16 **)((mlib_u8 *)zrtable +
			MLIB_POINTER_MUL(y));
		zz = (*(mlib_s16 *)((mlib_u8 *)zdata + (x >> 1)) - stz);
		c2 = tc2 >> MLIB_SHIFT;
		tc2 += dc2;
		c1 = tc1 >> MLIB_SHIFT;
		tc1 += dc1;
		c0 = tc >> MLIB_SHIFT;
		tc += dc;

		if (zz >= 0) {
			ux = (x - 4);
			dx = (((width - x) >> 31) & ~3);
			zdata = *(mlib_s16 **)((mlib_u8 *)zrtable +
				MLIB_POINTER_MUL(y));
			data = *(mlib_u8 **)((mlib_u8 *)rtable +
				MLIB_POINTER_MUL(y));
			udata = *(mlib_u8 **)((mlib_u8 *)rtable +
				MLIB_POINTER_MUL(y | ((ux >> 31) & ~3)));
			ddata = *(mlib_u8 **)((mlib_u8 *)rtable +
				MLIB_POINTER_MUL(y | dx));
			dx = (x + 4) | dx;
			offset = (t >> (MLIB_SHIFT - 8)) & 0xFF;
			*(mlib_s16 *)((mlib_u8 *)zdata + (x >> 1)) = stz;
			MLIB_ALIAS32(data, x, atable[offset + 256] << 8)
			MLIB_ALIAS32(udata, ux,	atable[offset ^ 0xFF] << 8)
			MLIB_ALIAS32(ddata, dx, atable[offset] << 8)
		}

		t += dt;
	}
}

/* *********************************************************** */

void
mlib_GraphicsSpamLineSet_ABGZ_32_InnVer(
	mlib_u8 **rtable,
	mlib_u8 **zrtable,
	mlib_s32 *coords,
	mlib_s32 width,
	mlib_s32 a)
{
	mlib_u8 *udata, *data, *ddata, *divt;
	mlib_s16 *zdata;
	mlib_s32 j, x, y, z, n, dt, dz, stz, zz, ux, dx;
	mlib_s32 c0, c1, c2, tc, dc, tc1, dc1, tc2, dc2;
	mlib_d64 A0, A1;

	A1 = (a & 0xFF) / 255.;
	A0 = 1. - A1;

	n = coords[MLIB_GDS_LENGTHLINES];
	y = 4 * coords[MLIB_GDS_STARTY];
	x = (~coords[MLIB_GDS_STARTX] >> (MLIB_SHIFT - 2)) & ~3;
	dt = coords[MLIB_GDS_STEPLINES];
	z = ~coords[MLIB_GDS_STARTZ];
	dz = coords[MLIB_GDS_STEPZ];
	tc2 = ~coords[MLIB_GDS_STARTC2];
	dc2 = coords[MLIB_GDS_STEPC2];
	tc1 = ~coords[MLIB_GDS_STARTC1];
	dc1 = coords[MLIB_GDS_STEPC1];
	tc = ~coords[MLIB_GDS_STARTC];
	dc = coords[MLIB_GDS_STEPC];
	dt = (dt >> (MLIB_SHIFT - 2)) & ~3;
	divt = mlib_GraphicsDivTab_u8 +
		(((dt) ? MLIB_DG_COEFF : MLIB_BG_COEFF) << 8);

	for (j = 0; j <= n; j++, y += 4, x += dt) {
		stz = z >> MLIB_SHIFT;
		z += dz;
		zdata = *(mlib_s16 **)((mlib_u8 *)zrtable +
			MLIB_POINTER_MUL(y));
		zz = *(mlib_s16 *)((mlib_u8 *)zdata + (x >> 1)) - stz;

		if (zz >= 0) {
			ux = (x - 4);
			dx = ((width - x) >> 31) & ~3;
			zdata = *(mlib_s16 **)((mlib_u8 *)zrtable +
				MLIB_POINTER_MUL(y));
			data = *(mlib_u8 **)((mlib_u8 *)rtable +
				MLIB_POINTER_MUL(y));
			udata = *(mlib_u8 **)((mlib_u8 *)rtable +
				MLIB_POINTER_MUL(y | ((ux >> 31) & ~3)));
			ddata = *(mlib_u8 **)((mlib_u8 *)rtable +
				MLIB_POINTER_MUL(y | dx));
			dx = (x + 4) | dx;
			*(mlib_s16 *)((mlib_u8 *)zdata + (x >> 1)) = stz;
			c2 = tc2 >> MLIB_SHIFT;
			tc2 += dc2;
			c1 = tc1 >> MLIB_SHIFT;
			tc1 += dc1;
			c0 = tc >> MLIB_SHIFT;
			tc += dc;
			MLIB_PUT_ABGZ_32(data, x);
			MLIB_SIMPLE_ALIAS32(udata, ux, divt);
			MLIB_SIMPLE_ALIAS32(ddata, dx, divt);
		} else {
			c2 = tc2 >> MLIB_SHIFT;
			tc2 += dc2;
			c1 = tc1 >> MLIB_SHIFT;
			tc1 += dc1;
			c0 = tc >> MLIB_SHIFT;
			tc += dc;
		}
	}
}

/* *********************************************************** */

void
mlib_GraphicsSpamLineSet_ABGZ_32(
	mlib_u8 **rtable,
	mlib_u8 **zrtable,
	mlib_s32 stride,
	mlib_s32 zstride,
	mlib_s32 *coords,
	mlib_s32 *coordsend,
	mlib_s32 a,
	mlib_s32 width,
	mlib_s32 height)
{
	mlib_s32 n, y, dt;

	n = stride;
	y = zstride;
	width *= 4;

	while (coords < coordsend) {
		n = coords[MLIB_GDS_LENGTHLINES];
		y = coords[MLIB_GDS_STARTY];
		dt = coords[MLIB_GDS_STEPLINES];

		if (n >= 0) {
			if (y < 0) {
				if (dt)
					mlib_GraphicsSpamLineSet_ABGZ_32_InnX
						(rtable, zrtable, coords,
						height, a);
				else
					mlib_GraphicsSpamLineSet_ABGZ_32_InnHor
						(rtable, zrtable, coords,
						height, a);
			} else {
				if (dt & (MLIB_ONE - 1))
					mlib_GraphicsSpamLineSet_ABGZ_32_InnY
						(rtable, zrtable, coords,
						width, a);
				else
					mlib_GraphicsSpamLineSet_ABGZ_32_InnVer
						(rtable, zrtable, coords,
						width, a);
			}
		}

		coords += MLIB_GDS_NEXT_GZ32;
	}
}

/* *********************************************************** */
