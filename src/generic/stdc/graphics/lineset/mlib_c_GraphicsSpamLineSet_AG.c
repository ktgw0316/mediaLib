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

#pragma ident	"@(#)mlib_c_GraphicsSpamLineSet_AG.c	9.2	07/10/09 SMI"

/*
 *  FUNCTIONS
 *    mlib_GraphicsSpamLineSet_AG_8  - line drawing function for mlib_u8 image
 *    mlib_GraphicsSpamLineSet_AG_32 - line drawing function for mlib_s32 image
 *
 *  SYNOPSIS
 *    void  mlib_GraphicsSpamLineSet_AG_8
 *                                    (mlib_u8  **rtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 width,
 *                                     mlib_s32 height);
 *    void  mlib_GraphicsSpamLineSet_AG_32
 *                                    (mlib_u8  **rtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 width,
 *                                     mlib_s32 height);
 *
 *
 *  ARGUMENTS
 *    rtable      Pointer to array of starts of row, rtable[-1][-1] is buffer
 *                  for unwanted stores
 *    stride      Stride of the image
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
 *    width       width = Image.Width - 2, is using to discover to
 *                clip neighbour in *Spam_A* functions
 *    height      height = Image.Height - 2, is using to discover to
 *                clip neighbour in *Spam_A* functions
 */

#include <mlib_graphics.h>
#include <mlib_GraphicsDrawSet.h>

/* *********************************************************** */

#ifdef _LP64

#define	MLIB_GET_POINTER(T)                                         \
	* (mlib_u8 **)((mlib_u8 *)rtable + (((T) >> (MLIB_SHIFT -   \
		3)) & ~7))

#define	MLIB_POINTER_MUL(X)	2*(X)

#else /* _LP64 */

#define	MLIB_GET_POINTER(T)                                         \
	* (mlib_u8 **)((mlib_u8 *)rtable + (((T) >> (MLIB_SHIFT -   \
		2)) & ~3))

#define	MLIB_POINTER_MUL(X)	(X)
#endif /* _LP64 */

/* *********************************************************** */

static void mlib_GraphicsSpamLineSet_AG_8_InnX(
	mlib_u8 **rtable,
	mlib_s32 *coords,
	mlib_s32 height);

static void mlib_GraphicsSpamLineSet_AG_8_InnHor(
	mlib_u8 **rtable,
	mlib_s32 *coords,
	mlib_s32 stride,
	mlib_s32 height);

static void mlib_GraphicsSpamLineSet_AG_8_InnY(
	mlib_u8 **rtable,
	mlib_s32 *coords,
	mlib_s32 stride,
	mlib_s32 width);

static void mlib_GraphicsSpamLineSet_AG_8_InnVer(
	mlib_u8 **rtable,
	mlib_s32 *coords,
	mlib_s32 stride,
	mlib_s32 width);

static void mlib_GraphicsSpamLineSet_AG_32_InnX(
	mlib_u8 **rtable,
	mlib_s32 *coords,
	mlib_s32 height);

static void mlib_GraphicsSpamLineSet_AG_32_InnHor(
	mlib_u8 **rtable,
	mlib_s32 *coords,
	mlib_s32 stride,
	mlib_s32 height);

static void mlib_GraphicsSpamLineSet_AG_32_InnY(
	mlib_u8 **rtable,
	mlib_s32 *coords,
	mlib_s32 stride,
	mlib_s32 width);

static void mlib_GraphicsSpamLineSet_AG_32_InnVer(
	mlib_u8 **rtable,
	mlib_s32 *coords,
	mlib_s32 width);

/* *********************************************************** */

void
mlib_GraphicsSpamLineSet_AG_8_InnX(
	mlib_u8 **rtable,
	mlib_s32 *coords,
	mlib_s32 height)
{
	mlib_u8 *atable, *table, *udata, *data, *ddata, *udivt, *divt, *ddivt;
	mlib_s32 j, x, y, c, n, t, dt, tc, dc, offset, zz, ux, dx;
	mlib_s32 bg, ubg, dbg;

	n = coords[MLIB_GDS_LENGTHLINES];
	t = ~coords[MLIB_GDS_STARTY];
	x = coords[MLIB_GDS_STARTX];
	dt = coords[MLIB_GDS_STEPLINES];
	tc = ~coords[MLIB_GDS_STARTC];
	dc = coords[MLIB_GDS_STEPC];
	zz = dt >> 31;
	zz = (dt ^ zz) - zz;
	c = tc >> MLIB_SHIFT;
	tc += dc;
	atable = mlib_GraphicsAliasTab_u8 +
		512 * mlib_GraphicsSqrtTab_u8[zz >> (MLIB_SHIFT - 7)];
	table = atable + 256;
	data = rtable[y = (t >> MLIB_SHIFT)];
	offset = (t >> (MLIB_SHIFT - 8)) & 0xFF;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j <= n; j++, x++) {
		divt = mlib_GraphicsDivTab_u8 + (table[offset] << 8);
		udivt = mlib_GraphicsDivTab_u8 + (atable[offset ^ 0xFF] << 8);
		ddivt = mlib_GraphicsDivTab_u8 + (atable[offset] << 8);
		y = t >> MLIB_SHIFT;
		t += dt;
		data = rtable[y];
		udata = rtable[y - 1];
		ddata = rtable[y + 1];
		ux = ((y - 1) >> 31) | x;
		dx = ((height - y) >> 31) | x;
		bg = data[x];
		ubg = udata[ux];
		dbg = ddata[dx];
		data[x] = bg + divt[c] - divt[bg];
		udata[ux] = ubg + udivt[c] - udivt[ubg];
		ddata[dx] = dbg + ddivt[c] - ddivt[dbg];
		offset = (t >> (MLIB_SHIFT - 8)) & 0xFF;
		c = tc >> MLIB_SHIFT;
		tc += dc;
	}
}

/* *********************************************************** */

void
mlib_GraphicsSpamLineSet_AG_8_InnHor(
	mlib_u8 **rtable,
	mlib_s32 *coords,
	mlib_s32 stride,
	mlib_s32 height)
{
	mlib_u8 *data, *divt;
	mlib_s32 j, x, y, n, tc, dc;
	mlib_s32 bg;

	n = coords[MLIB_GDS_LENGTHLINES];
	y = (~coords[MLIB_GDS_STARTY]) >> MLIB_SHIFT;
	x = coords[MLIB_GDS_STARTX];
	tc = ~coords[MLIB_GDS_STARTC];
	dc = coords[MLIB_GDS_STEPC];
	data = rtable[y];
	divt = mlib_GraphicsDivTab_u8 + (MLIB_BG_COEFF << 8);

	if (y > 0)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j <= n; j++) {
			bg = data[x + j - stride];
			data[x + j - stride] =
				bg + divt[(tc + j * dc) >> MLIB_SHIFT] -
				divt[bg];
		}

	if (y <= height)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j <= n; j++) {
			bg = data[x + j + stride];
			data[x + j + stride] =
				bg + divt[(tc + j * dc) >> MLIB_SHIFT] -
				divt[bg];
		}

	for (j = 0; j <= n; j++)
		data[x + j] = (tc + j * dc) >> MLIB_SHIFT;
}

/* *********************************************************** */

void
mlib_GraphicsSpamLineSet_AG_8_InnY(
	mlib_u8 **rtable,
	mlib_s32 *coords,
	mlib_s32 stride,
	mlib_s32 width)
{
	mlib_u8 *atable, *table, *udata, *data, *ddata, *udivt, *divt, *ddivt;
	mlib_s32 j, x, y, c, n, t, dt, tc, dc, offset, zz, ux, dx;
	mlib_s32 bg, ubg, dbg;

	n = coords[MLIB_GDS_LENGTHLINES];
	y = coords[MLIB_GDS_STARTY];
	t = ~coords[MLIB_GDS_STARTX];
	dt = coords[MLIB_GDS_STEPLINES];
	tc = ~coords[MLIB_GDS_STARTC];
	dc = coords[MLIB_GDS_STEPC];
	c = tc >> MLIB_SHIFT;
	tc += dc;
	zz = dt >> 31;
	zz = (dt ^ zz) - zz;
	atable = mlib_GraphicsAliasTab_u8 +
		512 * mlib_GraphicsSqrtTab_u8[zz >> (MLIB_SHIFT - 7)];
	table = atable + 256;
	data = rtable[y];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j <= n; j++, y++) {
		offset = (t >> (MLIB_SHIFT - 8)) & 0xFF;
		divt = mlib_GraphicsDivTab_u8 + (table[offset] << 8);
		udivt = mlib_GraphicsDivTab_u8 + (atable[offset ^ 0xFF] << 8);
		ddivt = mlib_GraphicsDivTab_u8 + (atable[offset] << 8);
		x = t >> MLIB_SHIFT;
		ux = x - 1;
		dx = (width - x) >> 31;
		t += dt;
		udata = rtable[y | (ux >> 31)];
		ddata = rtable[y | dx];
		dx = (x + 1) | dx;
		bg = data[x];
		ubg = udata[ux];
		dbg = ddata[dx];
		data[x] = bg + divt[c] - divt[bg];
		udata[ux] = ubg + udivt[c] - udivt[ubg];
		ddata[dx] = dbg + ddivt[c] - ddivt[dbg];
		data += stride;
		c = tc >> MLIB_SHIFT;
		tc += dc;
	}
}

/* *********************************************************** */

void
mlib_GraphicsSpamLineSet_AG_8_InnVer(
	mlib_u8 **rtable,
	mlib_s32 *coords,
	mlib_s32 stride,
	mlib_s32 width)
{
	mlib_u8 *udata, *data, *ddata, *divt;
	mlib_s32 j, x, y, c, n, dt, tc, dc, ux, dx;
	mlib_s32 alpha, ubg, dbg;

	n = coords[MLIB_GDS_LENGTHLINES];
	y = coords[MLIB_GDS_STARTY];
	x = (~coords[MLIB_GDS_STARTX]) >> MLIB_SHIFT;
	dt = coords[MLIB_GDS_STEPLINES];
	tc = ~coords[MLIB_GDS_STARTC];
	dc = coords[MLIB_GDS_STEPC];
	c = tc >> MLIB_SHIFT;
	tc += dc;
	data = rtable[y];
	alpha = (dt) ? MLIB_DG_COEFF : MLIB_BG_COEFF;
	dt = dt >> MLIB_SHIFT;
	divt = mlib_GraphicsDivTab_u8 + (alpha << 8);
	alpha = divt[c];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j <= n; j++, y++, x += dt) {
		alpha = divt[c];
		data[x] = c;
		c = tc >> MLIB_SHIFT;
		tc += dc;
		ux = x - 1;
		dx = (width - x) >> 31;
		udata = rtable[y | (ux >> 31)];
		ddata = rtable[y | dx];
		dx = (x + 1) | dx;
		ubg = udata[ux];
		dbg = ddata[dx];
		ubg = ubg + alpha - divt[ubg];
		dbg = dbg + alpha - divt[dbg];
		udata[ux] = ubg;
		ddata[dx] = dbg;
		data += stride;
	}
}

/* *********************************************************** */

void
mlib_GraphicsSpamLineSet_AG_8(
	mlib_u8 **rtable,
	mlib_s32 stride,
	mlib_s32 *coords,
	mlib_s32 *coordsend,
	mlib_s32 width,
	mlib_s32 height)
{
	mlib_s32 n, y, dt;

	while (coords < coordsend) {
		n = coords[MLIB_GDS_LENGTHLINES];
		y = coords[MLIB_GDS_STARTY];
		dt = coords[MLIB_GDS_STEPLINES];

		if (n >= 0) {
			if (y < 0) {
				if (dt)
					mlib_GraphicsSpamLineSet_AG_8_InnX
						(rtable, coords, height);
				else
					mlib_GraphicsSpamLineSet_AG_8_InnHor
						(rtable, coords, stride,
						height);
			} else {
				if (dt & (MLIB_ONE - 1))
					mlib_GraphicsSpamLineSet_AG_8_InnY
						(rtable, coords, stride, width);
				else
					mlib_GraphicsSpamLineSet_AG_8_InnVer
						(rtable, coords, stride, width);
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
	    mlib_s32 bg1, bg2, bg3;                                 \
	                                                            \
	    _data[3] = 0xFF;                                        \
	    bg1 = _data[2];                                         \
	    bg2 = _data[1];                                         \
	    bg1 = bg1 + _dtable[c2] - _dtable[bg1];                 \
	    _data[2] = bg1;                                         \
	    bg3 = _data[0];                                         \
	    bg2 = bg2 + _dtable[c1] - _dtable[bg2];                 \
	    bg3 = bg3 + _dtable[c0] - _dtable[bg3];                 \
	    _data[1] = bg2;                                         \
	    _data[0] = bg3;                                         \
	}

#define	MLIB_SIMPLE_ALIAS32(DATA, OFF, DIVT)                    \
	{                                                       \
	    mlib_u8 *_data = DATA + (OFF);                      \
	    mlib_s32 bg1, bg2, bg3;                             \
	                                                        \
	    _data[3] = 0xFF;                                    \
	    bg1 = _data[2];                                     \
	    bg2 = _data[1];                                     \
	    bg3 = _data[0];                                     \
	    bg1 = bg1 + DIVT[c2] - DIVT[bg1];                   \
	    bg2 = bg2 + DIVT[c1] - DIVT[bg2];                   \
	    bg3 = bg3 + DIVT[c0] - DIVT[bg3];                   \
	    _data[2] = bg1;                                     \
	    _data[1] = bg2;                                     \
	    _data[0] = bg3;                                     \
	}

#define	MLIB_PUT_G_32(DATA, OFF)                                \
	{                                                       \
	    mlib_u8 *_data = DATA + (OFF);                      \
	                                                        \
	    _data[3] = 0xFF;                                    \
	    _data[2] = c2;                                      \
	    _data[1] = c1;                                      \
	    _data[0] = c0;                                      \
	}

#else /* _LITTLE_ENDIAN */

#define	MLIB_ALIAS32(DATA, OFF, OFFSET)                             \
	{                                                           \
	    mlib_u8 *_dtable = mlib_GraphicsDivTab_u8 + (OFFSET);   \
	    mlib_u8 *_data = DATA + (OFF);                          \
	    mlib_s32 bg1, bg2, bg3;                                 \
	                                                            \
	    _data[0] = 0xFF;                                        \
	    bg1 = _data[1];                                         \
	    bg2 = _data[2];                                         \
	    bg1 = bg1 + _dtable[c2] - _dtable[bg1];                 \
	    _data[1] = bg1;                                         \
	    bg3 = _data[3];                                         \
	    bg2 = bg2 + _dtable[c1] - _dtable[bg2];                 \
	    bg3 = bg3 + _dtable[c0] - _dtable[bg3];                 \
	    _data[2] = bg2;                                         \
	    _data[3] = bg3;                                         \
	}

#define	MLIB_SIMPLE_ALIAS32(DATA, OFF, DIVT)                    \
	{                                                       \
	    mlib_u8 *_data = DATA + (OFF);                      \
	    mlib_s32 bg1, bg2, bg3;                             \
	                                                        \
	    _data[0] = 0xFF;                                    \
	    bg1 = _data[1];                                     \
	    bg2 = _data[2];                                     \
	    bg3 = _data[3];                                     \
	    bg1 = bg1 + DIVT[c2] - DIVT[bg1];                   \
	    bg2 = bg2 + DIVT[c1] - DIVT[bg2];                   \
	    bg3 = bg3 + DIVT[c0] - DIVT[bg3];                   \
	    _data[1] = bg1;                                     \
	    _data[2] = bg2;                                     \
	    _data[3] = bg3;                                     \
	}

#define	MLIB_PUT_G_32(DATA, OFF)                                \
	{                                                       \
	    mlib_u8 *_data = DATA + (OFF);                      \
	                                                        \
	    _data[0] = 0xFF;                                    \
	    _data[1] = c2;                                      \
	    _data[2] = c1;                                      \
	    _data[3] = c0;                                      \
	}

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

void
mlib_GraphicsSpamLineSet_AG_32_InnX(
	mlib_u8 **rtable,
	mlib_s32 *coords,
	mlib_s32 height)
{
	mlib_u8 *atable, *udata, *data, *ddata;
	mlib_s32 j, x, y, n, t, dt, offset, zz, ux, dx;
	mlib_s32 c0, c1, c2, tc, dc, tc1, dc1, tc2, dc2;

	n = coords[MLIB_GDS_LENGTHLINES];
	t = ~coords[MLIB_GDS_STARTY];
	x = coords[MLIB_GDS_STARTX];
	dt = coords[MLIB_GDS_STEPLINES];
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

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j <= n; j++, x++) {
		c2 = tc2 >> MLIB_SHIFT;
		tc2 += dc2;
		c1 = tc1 >> MLIB_SHIFT;
		tc1 += dc1;
		c0 = tc >> MLIB_SHIFT;
		tc += dc;
		y = t >> MLIB_SHIFT;
		data = rtable[y];
		udata = rtable[y - 1];
		ddata = rtable[y + 1];
		ux = ((y - 1) >> 31) | x;
		dx = ((height - y) >> 31) | x;
		offset = (t >> (MLIB_SHIFT - 8)) & 0xFF;
		t += dt;
		MLIB_ALIAS32(data, 4 * x, atable[offset + 256] << 8)
			MLIB_ALIAS32(udata, 4 * ux, atable[offset ^ 0xFF] << 8)
			MLIB_ALIAS32(ddata, 4 * dx, atable[offset] << 8)
	}
}

/* *********************************************************** */

void
mlib_GraphicsSpamLineSet_AG_32_InnHor(
	mlib_u8 **rtable,
	mlib_s32 *coords,
	mlib_s32 stride,
	mlib_s32 height)
{
	mlib_u8 *data, *dataend, *divt;
	mlib_s32 j, x, y, n, t;
	mlib_s32 c0, c1, c2, tc, dc, tc1, dc1, tc2, dc2;

	n = coords[MLIB_GDS_LENGTHLINES];
	t = ~coords[MLIB_GDS_STARTY];
	x = coords[MLIB_GDS_STARTX];
	tc2 = ~coords[MLIB_GDS_STARTC2];
	dc2 = coords[MLIB_GDS_STEPC2];
	tc1 = ~coords[MLIB_GDS_STARTC1];
	dc1 = coords[MLIB_GDS_STEPC1];
	tc = ~coords[MLIB_GDS_STARTC];
	dc = coords[MLIB_GDS_STEPC];
	data = rtable[y = (t >> MLIB_SHIFT)];
	divt = mlib_GraphicsDivTab_u8 + (MLIB_BG_COEFF << 8);

	if (y > 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j <= n; j++) {
			c2 = tc2 >> MLIB_SHIFT;
			tc2 += dc2;
			c1 = tc1 >> MLIB_SHIFT;
			tc1 += dc1;
			c0 = tc >> MLIB_SHIFT;
			tc += dc;
			MLIB_SIMPLE_ALIAS32(data, 4 * (x + j) - stride, divt)
		}

		tc2 = ~coords[MLIB_GDS_STARTC2];
		tc1 = ~coords[MLIB_GDS_STARTC1];
		tc = ~coords[MLIB_GDS_STARTC];
	}

	if (y <= height) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j <= n; j++) {
			c2 = tc2 >> MLIB_SHIFT;
			tc2 += dc2;
			c1 = tc1 >> MLIB_SHIFT;
			tc1 += dc1;
			c0 = tc >> MLIB_SHIFT;
			tc += dc;
			MLIB_SIMPLE_ALIAS32(data, 4 * (x + j) + stride, divt)
		}

		tc2 = ~coords[MLIB_GDS_STARTC2];
		tc1 = ~coords[MLIB_GDS_STARTC1];
		tc = ~coords[MLIB_GDS_STARTC];
	}

	tc2 |= (0xFF << (MLIB_SHIFT + 8));
	data += 4 * x;
	dataend = data + 4 * n;
	for (; data <= dataend - 4; data += 8) {
#ifdef _LITTLE_ENDIAN
		mlib_s32 tmp;

		tmp = tc2 >> MLIB_SHIFT;
		data[3] = (tmp >> 8) & 0xFF;
		data[2] = tmp & 0xFF;
		tc2 += dc2;
		data[1] = tc1 >> MLIB_SHIFT;
		tc1 += dc1;
		data[0] = tc >> MLIB_SHIFT;
		tc += dc;
		tmp = tc2 >> MLIB_SHIFT;
		data[7] = (tmp >> 8) & 0xFF;
		data[6] = tmp & 0xFF;
		tc2 += dc2;
		data[5] = tc1 >> MLIB_SHIFT;
		tc1 += dc1;
		data[4] = tc >> MLIB_SHIFT;
		tc += dc;
#else /* _LITTLE_ENDIAN */
		*(mlib_s16 *)data = tc2 >> MLIB_SHIFT;
		tc2 += dc2;
		data[2] = tc1 >> MLIB_SHIFT;
		tc1 += dc1;
		data[3] = tc >> MLIB_SHIFT;
		tc += dc;
		*(mlib_s16 *)(data + 4) = tc2 >> MLIB_SHIFT;
		tc2 += dc2;
		data[6] = tc1 >> MLIB_SHIFT;
		tc1 += dc1;
		data[7] = tc >> MLIB_SHIFT;
		tc += dc;
#endif /* _LITTLE_ENDIAN */
	}

	if (data <= dataend) {
#ifdef _LITTLE_ENDIAN
		mlib_s32 tmp = tc2 >> MLIB_SHIFT;

		data[3] = (tmp >> 8) & 0xFF;
		data[2] = tmp & 0xFF;
		tc2 += dc2;
		data[1] = tc1 >> MLIB_SHIFT;
		tc1 += dc1;
		data[0] = tc >> MLIB_SHIFT;
		tc += dc;
#else /* _LITTLE_ENDIAN */
		*(mlib_s16 *)data = tc2 >> MLIB_SHIFT;
		tc2 += dc2;
		data[2] = tc1 >> MLIB_SHIFT;
		tc1 += dc1;
		data[3] = tc >> MLIB_SHIFT;
		tc += dc;
#endif /* _LITTLE_ENDIAN */
	}
}

/* *********************************************************** */

void
mlib_GraphicsSpamLineSet_AG_32_InnY(
	mlib_u8 **rtable,
	mlib_s32 *coords,
	mlib_s32 stride,
	mlib_s32 width)
{
	mlib_u8 *atable, *udata, *data, *ddata;
	mlib_s32 j, x, y, n, t, dt, offset, zz, ux, dx;
	mlib_s32 c0, c1, c2, tc, dc, tc1, dc1, tc2, dc2;

	dt = stride;
	n = coords[MLIB_GDS_LENGTHLINES];
	y = 4 * coords[MLIB_GDS_STARTY];
	t = ~coords[MLIB_GDS_STARTX];
	dt = coords[MLIB_GDS_STEPLINES];
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

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j <= n; j++, y += 4) {
		c2 = tc2 >> MLIB_SHIFT;
		tc2 += dc2;
		c1 = tc1 >> MLIB_SHIFT;
		tc1 += dc1;
		c0 = tc >> MLIB_SHIFT;
		tc += dc;
		x = (t >> (MLIB_SHIFT - 2)) & ~3;
		ux = x - 4;
		dx = ((width - x) >> 31) & ~3;
		data = *(mlib_u8 **)((mlib_u8 *)rtable + MLIB_POINTER_MUL(y));
		udata = *(mlib_u8 **)((mlib_u8 *)rtable +
			MLIB_POINTER_MUL(y | ((ux >> 31) & ~3)));
		ddata = *(mlib_u8 **)((mlib_u8 *)rtable +
			MLIB_POINTER_MUL(y | dx));
		dx = (x + 4) | dx;
		offset = (t >> (MLIB_SHIFT - 8)) & 0xFF;
		t += dt;
		MLIB_ALIAS32(data, x, atable[offset + 256] << 8)
			MLIB_ALIAS32(udata, ux, atable[offset ^ 0xFF] << 8)
			MLIB_ALIAS32(ddata, dx, atable[offset] << 8)
	}
}

/* *********************************************************** */

void
mlib_GraphicsSpamLineSet_AG_32_InnVer(
	mlib_u8 **rtable,
	mlib_s32 *coords,
	mlib_s32 width)
{
	mlib_u8 *udata, *data, *ddata, *divt;
	mlib_s32 x, y, n, dt, ux, dx;
	mlib_s32 c0, c1, c2, tc, dc, tc1, dc1, tc2, dc2;

	n = 4 * coords[MLIB_GDS_LENGTHLINES];
	y = 4 * coords[MLIB_GDS_STARTY];
	x = ((~coords[MLIB_GDS_STARTX]) >> (MLIB_SHIFT - 2)) & ~3;
	dt = coords[MLIB_GDS_STEPLINES];
	tc2 = ~coords[MLIB_GDS_STARTC2];
	dc2 = coords[MLIB_GDS_STEPC2];
	tc1 = ~coords[MLIB_GDS_STARTC1];
	dc1 = coords[MLIB_GDS_STEPC1];
	tc = ~coords[MLIB_GDS_STARTC];
	dc = coords[MLIB_GDS_STEPC];
	divt = mlib_GraphicsDivTab_u8 +
		(((dt) ? MLIB_DG_COEFF : MLIB_BG_COEFF) << 8);
	n += y;
	dt = (dt >> (MLIB_SHIFT - 2)) & ~3;

	for (; y <= n; x += dt) {
		c2 = tc2 >> MLIB_SHIFT;
		tc2 += dc2;
		c1 = tc1 >> MLIB_SHIFT;
		tc1 += dc1;
		c0 = tc >> MLIB_SHIFT;
		tc += dc;
		ux = x - 4;
		dx = ((width - x) >> 31) & ~3;
		data = *(mlib_u8 **)((mlib_u8 *)rtable + MLIB_POINTER_MUL(y));
		udata = *(mlib_u8 **)((mlib_u8 *)rtable +
			MLIB_POINTER_MUL(y | ((ux >> 31) & ~3)));
		ddata = *(mlib_u8 **)((mlib_u8 *)rtable +
			MLIB_POINTER_MUL(y | dx));
		dx = (x + 4) | dx;
		y += 4;
		MLIB_PUT_G_32(data, x)
			MLIB_SIMPLE_ALIAS32(udata, ux, divt)
			MLIB_SIMPLE_ALIAS32(ddata, dx, divt)
	}
}

/* *********************************************************** */

void
mlib_GraphicsSpamLineSet_AG_32(
	mlib_u8 **rtable,
	mlib_s32 stride,
	mlib_s32 *coords,
	mlib_s32 *coordsend,
	mlib_s32 width,
	mlib_s32 height)
{
	mlib_s32 n, y, dt;

	width *= 4;

	while (coords < coordsend) {
		n = coords[MLIB_GDS_LENGTHLINES];
		y = coords[MLIB_GDS_STARTY];
		dt = coords[MLIB_GDS_STEPLINES];

		if (n >= 0) {
			if (y < 0) {
				if (dt)
					mlib_GraphicsSpamLineSet_AG_32_InnX
						(rtable, coords, height);
				else
					mlib_GraphicsSpamLineSet_AG_32_InnHor
						(rtable, coords, stride,
						height);
			} else {
				if (dt & (MLIB_ONE - 1))
					mlib_GraphicsSpamLineSet_AG_32_InnY
						(rtable, coords, stride, width);
				else
					mlib_GraphicsSpamLineSet_AG_32_InnVer
						(rtable, coords, width);
			}
		}

		coords += MLIB_GDS_NEXT_GZ32;
	}
}

/* *********************************************************** */
