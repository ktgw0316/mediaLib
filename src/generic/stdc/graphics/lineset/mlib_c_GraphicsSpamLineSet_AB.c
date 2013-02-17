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

#pragma ident	"@(#)mlib_c_GraphicsSpamLineSet_AB.c	9.2	07/10/09 SMI"

/*
 *  FUNCTIONS
 *    mlib_GraphicsSpamLineSet_AB_8  - line drawing function for mlib_u8 image
 *    mlib_GraphicsSpamLineSet_AB_32 - line drawing function for mlib_s32 image
 *
 *  SYNOPSIS
 *    void  mlib_GraphicsSpamLineSet_AB_8
 *                                    (mlib_u8  **rtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 color,
 *                                     mlib_s32 alpha,
 *                                     mlib_d64 dcolor,
 *                                     mlib_s32 width,
 *                                     mlib_s32 height);
 *    void  mlib_GraphicsSpamLineSet_AB_32
 *                                    (mlib_u8  **rtable,
 *                                     mlib_s32 stride,
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
 *    alpha	  alpha blending value
 *    dcolor      dcolor = vis_to_double_dup(color)
 *    width       width = Image.Width - 2, is using to discover to
 *                clip neighbour in *Spam_A* functions
 *    height      height = Image.Height - 2, is using to discover to
 *                clip neighbour in *Spam_A* functions
 */

#include <mlib_graphics.h>
#include <mlib_GraphicsDrawSet.h>
#include <mlib_GraphicsBlend.h>
#include <mlib_ImageDivTables.h>

/* *********************************************************** */

#ifdef _LP64

#define	MLIB_GET_POINTER(T)                                         \
	* (mlib_u8 **)((mlib_u8 *)rtable + (((T) >> (MLIB_SHIFT -   \
	    3)) & ~7))

#define	MLIB_POINTER_MUL(X)	2 * (X)

#else /* _LP64 */

#define	MLIB_GET_POINTER(T)                                         \
	* (mlib_u8 **)((mlib_u8 *)rtable + (((T) >> (MLIB_SHIFT -   \
	    2)) & ~3))

#define	MLIB_POINTER_MUL(X)	(X)
#endif /* _LP64 */

/* *********************************************************** */

#define	MLIB_BLENDING_8(s, c)                                   \
	(mlib_U82D64[s] * dA0) + (mlib_U82D64[c] * dA1);

/* *********************************************************** */

void
mlib_GraphicsSpamLineSet_AB_8(
    mlib_u8 **rtable,
    mlib_s32 stride,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 c,
    mlib_s32 a,
    mlib_d64 dc,
    mlib_s32 width,
    mlib_s32 height)
{
	mlib_d64 dA0, dA1;
	mlib_s32 j, x, y, n, t, dt, offset, zz, ux, dx;
	mlib_s32 ualpha, alpha, bg, ubg, dbg;
	mlib_u8 *atable, *table, *udata, *data, *ddata, *udivt, *divt, *ddivt;
	mlib_u8 cd, cud, cdd;

	c &= 0xff;
	a &= 0xff;
	dA1 = a / 255.;
	dA0 = 1. - dA1;

#ifdef __SUNPRO_C
#pragma unroll(1)
#endif /* __SUNPRO_C */
	for (; coords < coordsend; coords += MLIB_GDS_NEXT) {
		n = coords[MLIB_GDS_LENGTHLINES];
		y = coords[MLIB_GDS_STARTY];
		x = coords[MLIB_GDS_STARTX];
		dt = coords[MLIB_GDS_STEPLINES];
		zz = dt >> 31;
		zz = (dt ^ zz) - zz;
		atable = mlib_GraphicsAliasTab_u8 +
		    512 * mlib_GraphicsSqrtTab_u8[zz >> (MLIB_SHIFT - 7)];
		table = atable + 256;

		if (n >= 0) {
			if (y < 0) {
				t = ~y;
				data = rtable[y = (t >> MLIB_SHIFT)];

				if (dt) {
					offset = (t >> (MLIB_SHIFT - 8)) & 0xFF;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (j = 0; j <= n; j++, x++) {
						divt = mlib_GraphicsDivTab_u8 +
						    (table[offset] << 8);
						udivt = mlib_GraphicsDivTab_u8 +
						    (atable[offset ^ 0xFF]
						    << 8);
						ddivt = mlib_GraphicsDivTab_u8 +
						    (atable[offset] << 8);
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
						cd = bg + (divt[c] - divt[bg]);
						cud =
						    ubg + (udivt[c] -
						    udivt[ubg]);
						cdd =
						    dbg + (ddivt[c] -
						    ddivt[dbg]);

						data[x] =
						    MLIB_BLENDING_8(bg, cd);
						udata[ux] =
						    MLIB_BLENDING_8(ubg, cud);
						ddata[dx] =
						    MLIB_BLENDING_8(dbg, cdd);

						offset =
						    (t >> (MLIB_SHIFT -
						    8)) & 0xFF;
					}
				} else {
					divt = mlib_GraphicsDivTab_u8 +
					    (MLIB_BG_COEFF << 8);
					alpha = divt[c];

					if (y > 0)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
						for (j = 0; j <= n; j++) {
							bg = data[x + j -
							    stride];
							cd = bg + (alpha -
							    divt[bg]);
							data[x + j - stride] =
							    MLIB_BLENDING_8(bg,
							    cd);

						}

					if (y <= height)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
						for (j = 0; j <= n; j++) {
							bg = data[x + j +
							    stride];
							cd = bg + (alpha -
							    divt[bg]);
							data[x + j + stride] =
							    MLIB_BLENDING_8(bg,
							    cd);

						}

					data += x;
					ddata = data + n + 1;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (; data < ddata; data++) {
						bg = data[0];
						data[0] =
						    MLIB_BLENDING_8(bg, c);
					}
				}
			} else {
				data = rtable[y];
				t = ~x;
				x = t >> MLIB_SHIFT;

				if (dt & (MLIB_ONE - 1)) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (j = 0; j <= n; j++, y++) {
						offset = (t >> (MLIB_SHIFT -
						    8)) & 0xFF;
						divt = mlib_GraphicsDivTab_u8 +
						    (table[offset] << 8);
						udivt = mlib_GraphicsDivTab_u8 +
						    (atable[offset ^ 0xFF]
						    << 8);
						ddivt = mlib_GraphicsDivTab_u8 +
						    (atable[offset] << 8);
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
						cd = bg + (divt[c] - divt[bg]);
						cud =
						    ubg + (udivt[c] -
						    udivt[ubg]);
						cdd =
						    dbg + (ddivt[c] -
						    ddivt[dbg]);

						data[x] =
						    MLIB_BLENDING_8(bg, cd);
						udata[ux] =
						    MLIB_BLENDING_8(ubg, cud);
						ddata[dx] =
						    MLIB_BLENDING_8(dbg, cdd);

						data += stride;
					}
				} else {
					dt = dt >> MLIB_SHIFT;
					divt = mlib_GraphicsDivTab_u8 +
					    (atable[384] << 8);
					udivt = mlib_GraphicsDivTab_u8 +
					    (atable[128] << 8);
					alpha = divt[c];
					ualpha = udivt[c];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (j = 0; j <= n; j++, y++, x += dt) {
						ux = x - 1;
						dx = (width - x) >> 31;
						udata = rtable[y | (ux >> 31)];
						ddata = rtable[y | dx];
						dx = (x + 1) | dx;
						bg = data[x];
						ubg = udata[ux];
						dbg = ddata[dx];
						cud =
						    ubg + (ualpha - udivt[ubg]);
						cdd =
						    dbg + (ualpha - udivt[dbg]);
						data[x] =
						    MLIB_BLENDING_8(bg, c);
						udata[ux] =
						    MLIB_BLENDING_8(ubg, cud);
						ddata[dx] =
						    MLIB_BLENDING_8(dbg, cdd);
						data += stride;
					}
				}
			}
		}
	}
}

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN
#define	MLIB_ALIAS32(DATA, OFF, OFFSET)                           \
	{                                                         \
	    mlib_u8 bg1, bg2, bg3;                                \
	    mlib_u8 *_dtable = mlib_GraphicsDivTab_u8 + (OFFSET); \
	    mlib_u8 *_data = DATA + (OFF);                        \
	    mlib_u8 cbg1, cbg2, cbg3;                             \
	                                                          \
	    bg1 = _data[2];                                       \
	    bg2 = _data[1];                                       \
	    bg3 = _data[0];                                       \
	    cbg1 = bg1 + (_dtable[c2] - _dtable[bg1]);            \
	    cbg2 = bg2 + (_dtable[c1] - _dtable[bg2]);            \
	    cbg3 = bg3 + (_dtable[c0] - _dtable[bg3]);            \
	    _data[3] = 0xff;                                      \
	    _data[2] = MLIB_BLENDING_8(bg1, cbg1);                \
	    _data[1] = MLIB_BLENDING_8(bg2, cbg2);                \
	    _data[0] = MLIB_BLENDING_8(bg3, cbg3);                \
	}

#define	MLIB_SIMPLE_ALIAS32(DATA, OFF, DIVT)                      \
	{                                                         \
	    mlib_u8 bg1, bg2, bg3;                                \
	    mlib_u8 *_data = DATA + (OFF);                        \
	    mlib_u8 cbg1, cbg2, cbg3;                             \
	                                                          \
	    bg1 = _data[2];                                       \
	    bg2 = _data[1];                                       \
	    bg3 = _data[0];                                       \
	    cbg1 = bg1 + (alpha2 - DIVT[bg1]);                    \
	    cbg2 = bg2 + (alpha1 - DIVT[bg2]);                    \
	    cbg3 = bg3 + (alpha0 - DIVT[bg3]);                    \
	    _data[3] = 0xff;                                      \
	    _data[2] = MLIB_BLENDING_8(bg1, cbg1);                \
	    _data[1] = MLIB_BLENDING_8(bg2, cbg2);                \
	    _data[0] = MLIB_BLENDING_8(bg3, cbg3);                \
	}

#define	MLIB_SIMPLE_BLEND32(DATA)                                 \
	{                                                         \
	    mlib_u8 bg1, bg2, bg3;                                \
	    mlib_u8 cbg1, cbg2, cbg3;                             \
	    mlib_u8 *_data = (mlib_u8 *)(DATA);                   \
	                                                          \
	    bg1 = _data[2];                                       \
	    bg2 = _data[1];                                       \
	    bg3 = _data[0];                                       \
	    _data[3] = 0xff;                                      \
	    _data[2] = MLIB_BLENDING_8(bg1, c2);                  \
	    _data[1] = MLIB_BLENDING_8(bg2, c1);                  \
	    _data[0] = MLIB_BLENDING_8(bg3, c0);                  \
	}

#else /* _LITTLE_ENDIAN */

#define	MLIB_ALIAS32(DATA, OFF, OFFSET)                           \
	{                                                         \
	    mlib_u8 bg1, bg2, bg3;                                \
	    mlib_u8 cbg1, cbg2, cbg3;                             \
	    mlib_u8 *_dtable = mlib_GraphicsDivTab_u8 + (OFFSET); \
	    mlib_u8 *_data = DATA + (OFF);                        \
	                                                          \
	    bg1 = _data[1];                                       \
	    bg2 = _data[2];                                       \
	    bg3 = _data[3];                                       \
	    cbg1 = bg1 + (_dtable[c2] - _dtable[bg1]);            \
	    cbg2 = bg2 + (_dtable[c1] - _dtable[bg2]);            \
	    cbg3 = bg3 + (_dtable[c0] - _dtable[bg3]);            \
	    _data[0] = 0xff;                                      \
	    _data[1] = MLIB_BLENDING_8(bg1, cbg1);                \
	    _data[2] = MLIB_BLENDING_8(bg2, cbg2);                \
	    _data[3] = MLIB_BLENDING_8(bg3, cbg3);                \
	}

#define	MLIB_SIMPLE_ALIAS32(DATA, OFF, DIVT)                      \
	{                                                         \
	    mlib_u8 bg1, bg2, bg3;                                \
	    mlib_u8 cbg1, cbg2, cbg3;                             \
	    mlib_u8 *_data = DATA + (OFF);                        \
	                                                          \
	    bg1 = _data[1];                                       \
	    bg2 = _data[2];                                       \
	    bg3 = _data[3];                                       \
	    cbg1 = bg1 + (alpha2 - DIVT[bg1]);                    \
	    cbg2 = bg2 + (alpha1 - DIVT[bg2]);                    \
	    cbg3 = bg3 + (alpha0 - DIVT[bg3]);                    \
	    _data[0] = 0xff;                                      \
	    _data[1] = MLIB_BLENDING_8(bg1, cbg1);                \
	    _data[2] = MLIB_BLENDING_8(bg2, cbg2);                \
	    _data[3] = MLIB_BLENDING_8(bg3, cbg3);                \
	}

#define	MLIB_SIMPLE_BLEND32(DATA)                                 \
	{                                                         \
	    mlib_u8 bg1, bg2, bg3;                                \
	    mlib_u8 cbg1, cbg2, cbg3;                             \
	    mlib_u8 *_data = (mlib_u8 *)(DATA);                   \
	                                                          \
	    bg1 = _data[1];                                       \
	    bg2 = _data[2];                                       \
	    bg3 = _data[3];                                       \
	    _data[0] = 0xff;                                      \
	    _data[1] = MLIB_BLENDING_8(bg1, c2);                  \
	    _data[2] = MLIB_BLENDING_8(bg2, c1);                  \
	    _data[3] = MLIB_BLENDING_8(bg3, c0);                  \
	}

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

void
mlib_GraphicsSpamLineSet_AB_32(
    mlib_u8 **rtable,
    mlib_s32 stride,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 c,
    mlib_s32 a,
    mlib_d64 dc,
    mlib_s32 width,
    mlib_s32 height)
{
	mlib_d64 dA0, dA1;
	mlib_s32 j, x, y, n, t, dt, offset, zz, ux, dx;
	mlib_s32 alpha0, alpha1, alpha2;
	mlib_s32 c0, c1, c2;
	mlib_u8 *atable, *udata, *data, *ddata, *divt;

	a &= 0xff;
	dA1 = a / 255.;
	dA0 = 1. - dA1;

	x = dc;
	c0 = c & 0xFF;
	c1 = (c >> 8) & 0xFF;
	c2 = (c >> 16) & 0xFF;
	width *= 4;

	for (; coords < coordsend; coords += MLIB_GDS_NEXT) {
		n = coords[MLIB_GDS_LENGTHLINES];
		y = coords[MLIB_GDS_STARTY];
		x = coords[MLIB_GDS_STARTX];
		dt = coords[MLIB_GDS_STEPLINES];
		zz = dt >> 31;
		zz = (dt ^ zz) - zz;
		atable = mlib_GraphicsAliasTab_u8 +
		    512 * mlib_GraphicsSqrtTab_u8[zz >> (MLIB_SHIFT - 7)];

		if (n >= 0) {
			if (y < 0) {
				t = ~y;

				if (dt) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (j = 0; j <= n; j++, x++) {
						y = t >> MLIB_SHIFT;
						data = rtable[y];
						udata = rtable[y - 1];
						ddata = rtable[y + 1];
						ux = ((y - 1) >> 31) | x;
						dx = ((height - y) >> 31) | x;
						offset = (t >> (MLIB_SHIFT -
						    8)) & 0xFF;
						t += dt;
						MLIB_ALIAS32(data, 4 * x,
						    atable[offset + 256] << 8)
						    MLIB_ALIAS32(udata,
						    4 * ux,
						    atable[offset ^ 0xFF] << 8)
						    MLIB_ALIAS32(ddata,
						    4 * dx, atable[offset] << 8)
					}
				} else {
					data = rtable[y = (t >> MLIB_SHIFT)];
					divt = mlib_GraphicsDivTab_u8 +
					    (MLIB_BG_COEFF << 8);
					alpha0 = divt[c0];
					alpha1 = divt[c1];
					alpha2 = divt[c2];

					if (y > 0)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
						for (j = 0; j <= n; j++) {
							MLIB_SIMPLE_ALIAS32
							    (data,
							    4 * (x + j) -
							    stride, divt);
						}

					if (y <= height)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
						for (j = 0; j <= n; j++) {
							MLIB_SIMPLE_ALIAS32
							    (data,
							    4 * (x + j) +
							    stride, divt);
						}

					for (j = 0; j <= n; j++) {
						MLIB_SIMPLE_BLEND32(
						    (mlib_s32 *)(data +
						    4 * (x + j)));
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
						ux = x - 4;
						dx = ((width - x) >> 31) & ~3;
						data = *(mlib_u8 **)((mlib_u8 *)
						    rtable +
						    MLIB_POINTER_MUL(y));
						udata = *(mlib_u8 **)((mlib_u8
						    *)rtable +
						    MLIB_POINTER_MUL(y |
						    ((ux >> 31) & ~3)));
						ddata = *(mlib_u8 **)((mlib_u8
						    *)rtable +
						    MLIB_POINTER_MUL(y | dx));
						dx = (x + 4) | dx;
						offset = (t >> (MLIB_SHIFT -
						    8)) & 0xFF;
						t += dt;
						MLIB_ALIAS32(data, x,
						    atable[offset + 256] << 8)
						    MLIB_ALIAS32(udata, ux,
						    atable[offset ^ 0xFF] << 8)
						    MLIB_ALIAS32(ddata, dx,
						    atable[offset] << 8)
					}
				} else {
					y *= 4;
					data = *(mlib_u8 **)((mlib_u8 *)rtable +
					    MLIB_POINTER_MUL(y));
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
						ux = x - 4;
						dx = ((width - x) >> 31) & ~3;
						udata = *(mlib_u8 **)((mlib_u8
						    *)rtable +
						    MLIB_POINTER_MUL(y |
						    ((ux >> 31) & ~3)));
						ddata = *(mlib_u8 **)((mlib_u8
						    *)rtable +
						    MLIB_POINTER_MUL(y | dx));
						dx = (x + 4) | dx;
						MLIB_SIMPLE_BLEND32(
						    (mlib_s32 *)(data + x));
						MLIB_SIMPLE_ALIAS32(udata, ux,
						    divt);
						MLIB_SIMPLE_ALIAS32(ddata, dx,
						    divt);
						data += stride;
					}
				}
			}
		}
	}
}

/* *********************************************************** */
