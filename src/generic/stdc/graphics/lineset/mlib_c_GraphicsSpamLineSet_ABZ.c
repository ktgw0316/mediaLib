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

#pragma ident	"@(#)mlib_c_GraphicsSpamLineSet_ABZ.c	9.2	07/10/09 SMI"

/*
 *  FUNCTIONS
 *    mlib_GraphicsSpamLineSet_ABZ_8  - line drawing function for mlib_u8 image
 *    mlib_GraphicsSpamLineSet_ABZ_32 - line drawing function for mlib_s32 image
 *
 *  SYNOPSIS
 *    void  mlib_GraphicsSpamLineSet_ABZ_8
 *                                    (mlib_u8  **rtable,
 *                                     mlib_u8  **zrtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 zstride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 color,
 *                                     mlib_s32 alpha,
 *                                     mlib_d64 dcolor,
 *                                     mlib_s32 width,
 *                                     mlib_s32 height);
 *    void  mlib_GraphicsSpamLineSet_ABZ_32
 *                                    (mlib_u8  **rtable,
 *                                     mlib_u8  **zrtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 zstride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 color,
 *                                     mlib_s32 alpha,
 *                                     mlib_d64 dcolor,
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
 *    color       Color to draw, for MLIB_BYTE type of image color must be
 *                  the same in all bytes :
 *                    color  = (color & 0xFF),
 *                    color |= (color << 8),
 *                    color |= (color << 16)
 *    alpha       Alpha blending value
 *    width       width = Image.Width - 2, is using to discover to
 *                clip neighbour in *Spam_A* functions
 *    height      height = Image.Height - 2, is using to discover to
 *                clip neighbour in *Spam_A* functions
 */

#include <stdio.h>
#include <mlib_graphics.h>
#include <mlib_GraphicsDrawSet.h>
#include <mlib_GraphicsBlend.h>
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

#define	MLIB_BLENDING_8(s, c)                                   \
	(mlib_U82D64[s] * dA0) + (mlib_U82D64[c] * dA1);

/* *********************************************************** */

static void mlib_GraphicsSpamLineSet_ABZ_8_InnX(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 *coords,
    mlib_s32 c,
    mlib_s32 a,
    mlib_s32 height);

static void mlib_GraphicsSpamLineSet_ABZ_8_InnHor(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 *coords,
    mlib_s32 c,
    mlib_s32 a,
    mlib_s32 height);

static void mlib_GraphicsSpamLineSet_ABZ_8_InnY(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 *coords,
    mlib_s32 c,
    mlib_s32 a,
    mlib_s32 width);

static void mlib_GraphicsSpamLineSet_ABZ_8_InnVer(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 *coords,
    mlib_s32 c,
    mlib_s32 a,
    mlib_s32 width);

/* *********************************************************** */

void
mlib_GraphicsSpamLineSet_ABZ_8_InnX(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 *coords,
    mlib_s32 c,
    mlib_s32 a,
    mlib_s32 height)
{
	mlib_u8 *atable, *udata, *data, *ddata, *udivt, *divt, *ddivt;
	mlib_u8 cbg, cubg, cdbg;
	mlib_s16 *zdata;
	mlib_s32 j, x, y, z, n, t, dt, dz, stz, offset, zz, xz, ux, dx;
	mlib_s32 bg, ubg, dbg;
	mlib_d64 dA0, dA1;

	c &= 0xff;
	a &= 0xff;
	dA1 = a / 255.;
	dA0 = 1. - dA1;

	n = coords[MLIB_GDS_LENGTHLINES];
	t = ~coords[MLIB_GDS_STARTY];
	x = coords[MLIB_GDS_STARTX];
	dt = coords[MLIB_GDS_STEPLINES];
	z = ~coords[MLIB_GDS_STARTZ];
	dz = coords[MLIB_GDS_STEPZ];
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
		cbg = bg + divt[c] - divt[bg];
		cubg = ubg + udivt[c] - udivt[ubg];
		cdbg = dbg + ddivt[c] - ddivt[dbg];
		data[xz] = MLIB_BLENDING_8(bg, cbg);
		udata[ux] = MLIB_BLENDING_8(ubg, cubg);
		ddata[dx] = MLIB_BLENDING_8(dbg, cdbg);
	}
}

/* *********************************************************** */

void
mlib_GraphicsSpamLineSet_ABZ_8_InnHor(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 *coords,
    mlib_s32 c,
    mlib_s32 a,
    mlib_s32 height)
{
	mlib_u8 *udata, *data, *ddata, *divt;
	mlib_u8 cbg, cubg, cdbg;
	mlib_s16 *zdata, *zdata1;
	mlib_s32 j, x, y, z, n, dz, stz, zz, xz, ux, dx;
	mlib_s32 ualpha, alpha, dalpha, bg, ubg, dbg;
	mlib_d64 dA0, dA1;

	c &= 0xff;
	a &= 0xff;
	dA1 = a / 255.;
	dA0 = 1. - dA1;

	n = coords[MLIB_GDS_LENGTHLINES];
	y = (~coords[MLIB_GDS_STARTY]) >> MLIB_SHIFT;
	x = coords[MLIB_GDS_STARTX];
	z = ~coords[MLIB_GDS_STARTZ];
	dz = coords[MLIB_GDS_STEPZ];
	zdata = (mlib_s16 *)(zrtable[y]);
	divt = mlib_GraphicsDivTab_u8 + (MLIB_BG_COEFF << 8);
	ualpha = (y - 1) >> 31;
	dalpha = (height - y) >> 31;
	alpha = divt[c];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
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
		zdata1[xz] = stz;
		bg = data[xz];
		ubg = udata[ux];
		dbg = ddata[dx];
		cubg = ubg + alpha - divt[ubg];
		cdbg = dbg + alpha - divt[dbg];
		data[xz] = MLIB_BLENDING_8(bg, c);
		udata[ux] = MLIB_BLENDING_8(ubg, cubg);
		ddata[dx] = MLIB_BLENDING_8(dbg, cdbg);
	}
}

/* *********************************************************** */

void
mlib_GraphicsSpamLineSet_ABZ_8_InnY(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 *coords,
    mlib_s32 c,
    mlib_s32 a,
    mlib_s32 width)
{
	mlib_u8 *atable, *udata, *data, *ddata, *udivt, *divt, *ddivt;
	mlib_s16 *zdata;
	mlib_s32 x, y, z, n, t, dt, dz, stz, offset, zz, xz, ux, dx;
	mlib_s32 bg, ubg, dbg;
	mlib_u8 cbg, cubg, cdbg;
	mlib_d64 dA0, dA1;

	c &= 0xff;
	a &= 0xff;
	dA1 = a / 255.;
	dA0 = 1. - dA1;

	n = coords[MLIB_GDS_LENGTHLINES];
	y = coords[MLIB_GDS_STARTY];
	t = ~coords[MLIB_GDS_STARTX];
	dt = coords[MLIB_GDS_STEPLINES];
	z = ~coords[MLIB_GDS_STARTZ];
	dz = coords[MLIB_GDS_STEPZ];
	zz = dt >> 31;
	zz = (dt ^ zz) - zz;
	atable = mlib_GraphicsAliasTab_u8 +
	    512 * mlib_GraphicsSqrtTab_u8[zz >> (MLIB_SHIFT - 7)];
	n += y;

	for (; y <= n; ) {
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
		y++;
		bg = data[xz];
		ubg = udata[ux];
		dbg = ddata[dx];
		zdata[xz] = stz;
		cbg = bg + divt[c] - divt[bg];
		cubg = ubg + udivt[c] - udivt[ubg];
		cdbg = dbg + ddivt[c] - ddivt[dbg];
		data[xz] = MLIB_BLENDING_8(bg, cbg);
		udata[ux] = MLIB_BLENDING_8(ubg, cubg);
		ddata[dx] = MLIB_BLENDING_8(dbg, cdbg);
	}
}

/* *********************************************************** */

void
mlib_GraphicsSpamLineSet_ABZ_8_InnVer(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 *coords,
    mlib_s32 c,
    mlib_s32 a,
    mlib_s32 width)
{
	mlib_u8 *udata, *data, *ddata, *divt;
	mlib_s16 *zdata;
	mlib_s32 j, x, y, z, n, dt, dz, stz, zz, xz, ux, dx;
	mlib_s32 alpha, bg, ubg, dbg;
	mlib_u8 cbg, cubg, cdbg;
	mlib_d64 dA0, dA1;

	c &= 0xff;
	a &= 0xff;
	dA1 = a / 255.;
	dA0 = 1. - dA1;

	n = coords[MLIB_GDS_LENGTHLINES];
	y = coords[MLIB_GDS_STARTY];
	x = (~coords[MLIB_GDS_STARTX]) >> MLIB_SHIFT;
	dt = coords[MLIB_GDS_STEPLINES];
	z = ~coords[MLIB_GDS_STARTZ];
	dz = coords[MLIB_GDS_STEPZ];
	alpha = (dt) ? MLIB_DG_COEFF : MLIB_BG_COEFF;
	dt = dt >> MLIB_SHIFT;
	divt = mlib_GraphicsDivTab_u8 + (alpha << 8);
	alpha = divt[c];

#ifdef __SUNPRO_C
#pragma unroll(1)
#endif /* __SUNPRO_C */
	for (j = 0; j <= n; j++, x += dt) {
		stz = z >> MLIB_SHIFT;
		z += dz;
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
		zdata[xz] = stz;
		bg = data[xz];
		ubg = udata[ux];
		dbg = ddata[dx];
		cubg = ubg + alpha - divt[ubg];
		cdbg = dbg + alpha - divt[dbg];
		data[xz] = MLIB_BLENDING_8(bg, c);
		udata[ux] = MLIB_BLENDING_8(ubg, cubg);
		ddata[dx] = MLIB_BLENDING_8(dbg, cdbg);
		y++;
	}
}

/* *********************************************************** */

void
mlib_GraphicsSpamLineSet_ABZ_8(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 stride,
    mlib_s32 zstride,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 c,
    mlib_s32 a,
    mlib_d64 dc,
    mlib_s32 width,
    mlib_s32 height)
{
	mlib_s32 n, y, dt;

	n = stride;
	y = zstride;
	dt = dc;

	c &= 0xFF;

	while (coords < coordsend) {
		n = coords[MLIB_GDS_LENGTHLINES];
		y = coords[MLIB_GDS_STARTY];
		dt = coords[MLIB_GDS_STEPLINES];

		if (n >= 0) {
			if (y < 0) {
				if (dt)
					mlib_GraphicsSpamLineSet_ABZ_8_InnX
					    (rtable, zrtable, coords, c, a,
					    height);
				else
					mlib_GraphicsSpamLineSet_ABZ_8_InnHor
					    (rtable, zrtable, coords, c, a,
					    height);
			} else {
				if (dt & (MLIB_ONE - 1))
					mlib_GraphicsSpamLineSet_ABZ_8_InnY
					    (rtable, zrtable, coords, c, a,
					    width);
				else
					mlib_GraphicsSpamLineSet_ABZ_8_InnVer
					    (rtable, zrtable, coords, c, a,
					    width);
			}
		}

		coords += MLIB_GDS_NEXT_GZ;
	}
}

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN
#define	MLIB_ALIAS32(DATA, OFF, OFFSET)                             \
	{                                                           \
	    mlib_u8 bg1, bg2, bg3;                                  \
	    mlib_u8 *_dtable = mlib_GraphicsDivTab_u8 + (OFFSET);   \
	    mlib_u8 *_data = DATA + (OFF);                          \
	    mlib_u8 cbg1, cbg2, cbg3;                               \
	                                                            \
	    bg1 = _data[2];                                         \
	    bg2 = _data[1];                                         \
	    bg3 = _data[0];                                         \
	    cbg1 = bg1 + (_dtable[c2] - _dtable[bg1]);              \
	    cbg2 = bg2 + (_dtable[c1] - _dtable[bg2]);              \
	    cbg3 = bg3 + (_dtable[c0] - _dtable[bg3]);              \
	    _data[3] = 0xff;                                        \
	    _data[2] = MLIB_BLENDING_8(bg1, cbg1);                  \
	    _data[1] = MLIB_BLENDING_8(bg2, cbg2);                  \
	    _data[0] = MLIB_BLENDING_8(bg3, cbg3);                  \
	}

#define	MLIB_SIMPLE_ALIAS32(DATA, OFF, DIVT)                    \
	{                                                       \
	    mlib_u8 bg1, bg2, bg3;                              \
	    mlib_u8 *_data = DATA + (OFF);                      \
	    mlib_u8 cbg1, cbg2, cbg3;                           \
	                                                        \
	    bg1 = _data[2];                                     \
	    bg2 = _data[1];                                     \
	    bg3 = _data[0];                                     \
	    cbg1 = bg1 + (alpha2 - DIVT[bg1]);                  \
	    cbg2 = bg2 + (alpha1 - DIVT[bg2]);                  \
	    cbg3 = bg3 + (alpha0 - DIVT[bg3]);                  \
	    _data[3] = 0xff;                                    \
	    _data[2] = MLIB_BLENDING_8(bg1, cbg1);              \
	    _data[1] = MLIB_BLENDING_8(bg2, cbg2);              \
	    _data[0] = MLIB_BLENDING_8(bg3, cbg3);              \
	}

#define	MLIB_SIMPLE_BLEND32(DATA)                               \
	{                                                       \
	    mlib_u8 bg1, bg2, bg3;                              \
	    mlib_u8 cbg1, cbg2, cbg3;                           \
	    mlib_u8 *_data = (mlib_u8 *)(DATA);                 \
	                                                        \
	    bg1 = _data[2];                                     \
	    bg2 = _data[1];                                     \
	    bg3 = _data[0];                                     \
	    _data[3] = 0xff;                                    \
	    _data[2] = MLIB_BLENDING_8(bg1, c2);                \
	    _data[1] = MLIB_BLENDING_8(bg2, c1);                \
	    _data[0] = MLIB_BLENDING_8(bg3, c0);                \
	}

#else /* _LITTLE_ENDIAN */

#define	MLIB_ALIAS32(DATA, OFF, OFFSET)                             \
	{                                                           \
	    mlib_u8 bg1, bg2, bg3;                                  \
	    mlib_u8 cbg1, cbg2, cbg3;                               \
	    mlib_u8 *_dtable = mlib_GraphicsDivTab_u8 + (OFFSET);   \
	    mlib_u8 *_data = DATA + (OFF);                          \
	                                                            \
	    bg1 = _data[1];                                         \
	    bg2 = _data[2];                                         \
	    bg3 = _data[3];                                         \
	    cbg1 = bg1 + (_dtable[c2] - _dtable[bg1]);              \
	    cbg2 = bg2 + (_dtable[c1] - _dtable[bg2]);              \
	    cbg3 = bg3 + (_dtable[c0] - _dtable[bg3]);              \
	    _data[0] = 0xff;                                        \
	    _data[1] = MLIB_BLENDING_8(bg1, cbg1);                  \
	    _data[2] = MLIB_BLENDING_8(bg2, cbg2);                  \
	    _data[3] = MLIB_BLENDING_8(bg3, cbg3);                  \
	}

#define	MLIB_SIMPLE_ALIAS32(DATA, OFF, DIVT)                    \
	{                                                       \
	    mlib_u8 bg1, bg2, bg3;                              \
	    mlib_u8 cbg1, cbg2, cbg3;                           \
	    mlib_u8 *_data = DATA + (OFF);                      \
	                                                        \
	    bg1 = _data[1];                                     \
	    bg2 = _data[2];                                     \
	    bg3 = _data[3];                                     \
	    cbg1 = bg1 + (alpha2 - DIVT[bg1]);                  \
	    cbg2 = bg2 + (alpha1 - DIVT[bg2]);                  \
	    cbg3 = bg3 + (alpha0 - DIVT[bg3]);                  \
	    _data[0] = 0xff;                                    \
	    _data[1] = MLIB_BLENDING_8(bg1, cbg1);              \
	    _data[2] = MLIB_BLENDING_8(bg2, cbg2);              \
	    _data[3] = MLIB_BLENDING_8(bg3, cbg3);              \
	}

#define	MLIB_SIMPLE_BLEND32(DATA)                               \
	{                                                       \
	    mlib_u8 bg1, bg2, bg3;                              \
	    mlib_u8 cbg1, cbg2, cbg3;                           \
	    mlib_u8 *_data = (mlib_u8 *)(DATA);                 \
	                                                        \
	    bg1 = _data[1];                                     \
	    bg2 = _data[2];                                     \
	    bg3 = _data[3];                                     \
	    _data[0] = 0xff;                                    \
	    _data[1] = MLIB_BLENDING_8(bg1, c2);                \
	    _data[2] = MLIB_BLENDING_8(bg2, c1);                \
	    _data[3] = MLIB_BLENDING_8(bg3, c0);                \
	}

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

void
mlib_GraphicsSpamLineSet_ABZ_32(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 stride,
    mlib_s32 zstride,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 c,
    mlib_s32 a,
    mlib_d64 dc,
    mlib_s32 width,
    mlib_s32 height)
{
	mlib_s32 c0, c1, c2;
	mlib_d64 dA0, dA1;

	a &= 0xff;
	dA1 = a / 255.;
	dA0 = 1. - dA1;

	c0 = stride | zstride | (mlib_s32)dc;

	c0 = c & 0xFF;
	c1 = (c >> 8) & 0xFF;
	c2 = (c >> 16) & 0xFF;
	width *= 4;

	while (coords < coordsend) {
		mlib_u8 *atable, *udata, *data, *ddata, *divt;
		mlib_s16 *zdata, *zdata1;
		mlib_s32 j, x, y, z, n, t, dt, dz, stz, offset, zz, ux, dx, xz;
		mlib_s32 alpha0, alpha1, alpha2, ualpha, dalpha;

		n = coords[MLIB_GDS_LENGTHLINES];
		y = coords[MLIB_GDS_STARTY];
		x = coords[MLIB_GDS_STARTX];
		dt = coords[MLIB_GDS_STEPLINES];
		z = ~coords[MLIB_GDS_STARTZ];
		dz = coords[MLIB_GDS_STEPZ];
		coords += MLIB_GDS_NEXT_GZ;
		zz = dt >> 31;
		zz = (dt ^ zz) - zz;
		atable = mlib_GraphicsAliasTab_u8 +
		    512 * mlib_GraphicsSqrtTab_u8[zz >> (MLIB_SHIFT - 7)];
		stz = z >> MLIB_SHIFT;
		z += dz;

		if (n >= 0) {
			if (y < 0) {
				t = ~y;

				if (dt) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (j = 0; j <= n; j++, x++) {
						zdata = (mlib_s16
						    *)(MLIB_GET_POINTER
						    (zrtable, t));
						zz = (zdata[x] - stz) >> 31;
						y = t >> MLIB_SHIFT;
						zdata = (mlib_s16 *)(zrtable[y |
						    zz]);
						data = rtable[y | zz];
						udata = rtable[(y - 1) | zz];
						ddata = rtable[(y + 1) | zz];
						xz = x | zz;
						ux = ((y - 1) >> 31) | xz;
						dx = ((height - y) >> 31) | xz;
						offset = (t >> (MLIB_SHIFT -
						    8)) & 0xFF;
						t += dt;
						zdata[xz] = stz;
						MLIB_ALIAS32(data, 4 * xz,
						    atable[offset + 256] << 8);
						MLIB_ALIAS32(udata, 4 * ux,
						    atable[offset ^ 0xFF] << 8);
						MLIB_ALIAS32(ddata, 4 * dx,
						    atable[offset] << 8);
						stz = z >> MLIB_SHIFT;
						z += dz;
					}
				} else {
					zdata = (mlib_s16 *)(zrtable[y =
					    t >> MLIB_SHIFT]);
					divt = mlib_GraphicsDivTab_u8 +
					    (MLIB_BG_COEFF << 8);
					alpha0 = divt[c0];
					alpha1 = divt[c1];
					alpha2 = divt[c2];
					ualpha = (y - 1) >> 31;
					dalpha = (height - y) >> 31;

#ifdef __SUNPRO_C
#pragma unroll(1)
#endif /* __SUNPRO_C */
					for (j = 0; j <= n; j++, x++) {
						zz = (zdata[x] - stz) >> 31;
						ux = ualpha | zz;
						dx = dalpha | zz;
						data = rtable[y | zz];
						zdata1 = (mlib_s16 *)(zrtable[y
						    | zz]);
						udata = rtable[(y - 1) | ux];
						ddata = rtable[(y + 1) | dx];
						xz = x | zz;
						ux = x | ux;
						dx = x | dx;
						MLIB_SIMPLE_ALIAS32(udata,
						    4 * ux, divt);
						MLIB_SIMPLE_ALIAS32(ddata,
						    4 * dx, divt);
						zdata1[xz] = stz;
						MLIB_SIMPLE_BLEND32(
						    (mlib_s32 *)(data +
						    4 * xz));
						stz = z >> MLIB_SHIFT;
						z += dz;
					}
				}
			} else {
				t = ~x;

				if (dt & (MLIB_ONE - 1)) {
					y *= 4;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (j = 0; j <= n; j++, y += 4) {
						x = (t >> (MLIB_SHIFT -
						    2)) & ~3;
						zdata = *(mlib_s16 **)((mlib_u8
						    *)zrtable +
						    MLIB_POINTER_MUL(y));
						zz = ((*(mlib_s16 *)((mlib_u8 *)
						    zdata + (x >> 1)) -
						    stz) >> 31) & ~3;
						ux = (x - 4) | zz;
						dx = (((width -
						    x) >> 31) & ~3) | zz;
						zdata = *(mlib_s16 **)((mlib_u8
						    *)zrtable +
						    MLIB_POINTER_MUL(y | zz));
						data = *(mlib_u8 **)((mlib_u8 *)
						    rtable +
						    MLIB_POINTER_MUL(y | zz));
						udata = *(mlib_u8 **)((mlib_u8
						    *)rtable +
						    MLIB_POINTER_MUL(y |
						    ((ux >> 31) & ~3)));
						ddata = *(mlib_u8 **)((mlib_u8
						    *)rtable +
						    MLIB_POINTER_MUL(y | dx));
						dx = (x + 4) | dx;
						xz = x | zz;
						offset = (t >> (MLIB_SHIFT -
						    8)) & 0xFF;
						t += dt;
						*(mlib_s16 *)((mlib_u8 *)zdata +
						    (xz >> 1)) = stz;
						MLIB_ALIAS32(data, xz,
						    atable[offset + 256] << 8);
						MLIB_ALIAS32(udata, ux,
						    atable[offset ^ 0xFF] << 8);
						MLIB_ALIAS32(ddata, dx,
						    atable[offset] << 8);
						stz = z >> MLIB_SHIFT;
						z += dz;
					}
				} else {
					y *= 4;
					x = (t >> (MLIB_SHIFT - 2)) & ~3;
					dt = (dt >> (MLIB_SHIFT - 2)) & ~3;
					divt = mlib_GraphicsDivTab_u8 +
					    (atable[128] << 8);
					alpha0 = divt[c0];
					alpha1 = divt[c1];
					alpha2 = divt[c2];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (j = 0; j <= n;
					    j++, y += 4, x += dt) {
						zdata = *(mlib_s16 **)((mlib_u8
						    *)zrtable +
						    MLIB_POINTER_MUL(y));
						zz = ((*(mlib_s16 *)((mlib_u8 *)
						    zdata + (x >> 1)) -
						    stz) >> 31) & ~3;
						ux = (x - 4) | zz;
						dx = (((width -
						    x) >> 31) & ~3) | zz;
						zdata = *(mlib_s16 **)((mlib_u8
						    *)zrtable +
						    MLIB_POINTER_MUL(y | zz));
						data = *(mlib_u8 **)((mlib_u8 *)
						    rtable +
						    MLIB_POINTER_MUL(y | zz));
						udata = *(mlib_u8 **)((mlib_u8
						    *)rtable +
						    MLIB_POINTER_MUL(y |
						    ((ux >> 31) & ~3)));
						ddata = *(mlib_u8 **)((mlib_u8
						    *)rtable +
						    MLIB_POINTER_MUL(y | dx));
						dx = (x + 4) | dx;
						xz = x | zz;
						*(mlib_s16 *)((mlib_u8 *)zdata +
						    (xz >> 1)) = stz;
						MLIB_SIMPLE_BLEND32(
						    (mlib_s32 *)((mlib_u8 *)data
						    + xz));
						MLIB_SIMPLE_ALIAS32(udata, ux,
						    divt);
						MLIB_SIMPLE_ALIAS32(ddata, dx,
						    divt);
						stz = z >> MLIB_SHIFT;
						z += dz;
					}
				}
			}
		}
	}
}

/* *********************************************************** */
