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

#pragma ident	"@(#)mlib_c_GraphicsSpamLineSet_BZ.c	9.2	07/10/09 SMI"

/*
 *  FUNCTIONS
 *    mlib_GraphicsSpamLineSet_BZ_8  - line drawing function
 *                                     with Z - buffer mode and blending
 *    mlib_GraphicsSpamLineSet_BZ_32 - line drawing function
 *                                     with Z - buffer mode and blending
 *
 *  SYNOPSIS
 *    void  mlib_GraphicsSpamLineSet_BZ_8
 *                                    (mlib_u8  **rtable,
 *                                     mlib_u8  **zrtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 zstride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 color,
 *                                     mlib_s32 alpha,
 *                                     mlib_d64 dcolor);
 *    void  mlib_GraphicsSpamLineSet_BZ_32
 *                                    (mlib_u8  **rtable,
 *                                     mlib_u8  **zrtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 zstride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 color,
 *                                     mlib_s32 alpha,
 *                                     mlib_d64 dcolor);
 *
 *  ARGUMENTS
 *    rtable      Pointer to array of starts of row, rtable[-1][-1] is buffer
 *                  for unwanted stores
 *    zrtable     Pointer to array of starts of row of zbuffer, zrtable[-1][-1]
 *                  is buffer for unwanted stores
 *    stride      Stride of the image
 *    zstride     Stride of the zbuffer image
 *    coords      Array of values sets using in *Spam* functions:
 *                  coords[MLIB_GDS_STARTX + i * MLIB_GDS_NEXT_GZ]      holds X
 *                  coords[MLIB_GDS_STARTY + i * MLIB_GDS_NEXT_GZ]      holds Y
 *                  coords[MLIB_GDS_LENGTHLINES + i * MLIB_GDS_NEXT_GZ] holds LL
 *                  coords[MLIB_GDS_STEPLINES + i * MLIB_GDS_NEXT_GZ]   holds SL
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
 *   coordsend    for(; coords < coordsend; coords += MLIB_GDS_NEXT_GZ)
 *   color        Color to draw, for MLIB_BYTE type of image color must be
 *                  the same in all bytes :
 *                    c = (color & 0xFF),
 *                    c |= (c << 8),
 *                    c |= (c << 16)
 *   alpha        Alpha blending value
 */

#include <stdlib.h>
#include <stdio.h>
#include <mlib_graphics.h>
#include <mlib_GraphicsDrawSet.h>
#include <mlib_ImageDivTables.h>

/* *********************************************************** */

#ifdef _LP64

/* *********************************************************** */

#define	MLIB_GET_POINTER(RT, T)                                            \
	* (mlib_u8 **)((mlib_u8 *)RT + (((T) >> (MLIB_SHIFT - 3)) & ~7))

/* *********************************************************** */

#define	MLIB_GET_POINTER2(RT, T, ZZ)                             \
	* (mlib_u8 **)((mlib_u8 *)RT + ((((T) >> (MLIB_SHIFT -   \
	    3)) | ZZ) & ~7))

/* *********************************************************** */

#define	MLIB_POINTER_COEF	8

/* *********************************************************** */

#else /* _LP64 */

/* *********************************************************** */

#define	MLIB_GET_POINTER(RT, T)                                            \
	* (mlib_u8 **)((mlib_u8 *)RT + (((T) >> (MLIB_SHIFT - 2)) & ~3))

/* *********************************************************** */

#define	MLIB_GET_POINTER2(RT, T, ZZ)                             \
	* (mlib_u8 **)((mlib_u8 *)RT + ((((T) >> (MLIB_SHIFT -   \
	    2)) | ZZ) & ~3))

/* *********************************************************** */

#define	MLIB_POINTER_COEF	4

/* *********************************************************** */

#endif /* _LP64 */

/* *********************************************************** */

#define	MLIB_BLENDING(s, ca)	s = (mlib_U82D64[s] * A1) + ca;

/* *********************************************************** */

void
mlib_GraphicsSpamLineSet_BZ_8(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 stride,
    mlib_s32 zstride,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 c,
    mlib_s32 a,
    mlib_d64 dc)
{
	mlib_u8 *data;
	mlib_s16 *zdata, *zdata1;
	mlib_s32 j, x, y, z, n, t, dt, dz, zz, stz, sdt, xz;
	mlib_d64 CA, A1, d;

	c &= 0xff;
	a &= 0xff;

	A1 = ((255. - a) / 255.);
	CA = c * a / 255.;

	x = stride;
	y = dc;

	for (; coords < coordsend; coords += MLIB_GDS_NEXT_GZ) {
		n = coords[MLIB_GDS_LENGTHLINES];
		y = coords[MLIB_GDS_STARTY];
		x = coords[MLIB_GDS_STARTX];
		z = ~coords[MLIB_GDS_STARTZ];
		dz = coords[MLIB_GDS_STEPZ];
		dt = coords[MLIB_GDS_STEPLINES];

		if (n >= 0) {
			if (y < 0) {
				y = ~y;
				data = rtable[y >> MLIB_SHIFT];
				zdata = (mlib_s16 *)(zrtable[y >> MLIB_SHIFT]);

				if (dt) {
					t = y;
					stz = z >> MLIB_SHIFT;
					z += dz;
					data = rtable[-1];
					xz = -1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (j = 0; j <= n; j++) {
						zdata = (mlib_s16
						    *)(MLIB_GET_POINTER
						    (zrtable, t));
						zz = (zdata[x] - stz) >> 31;
						MLIB_BLENDING(data[xz], CA);
						zdata1 = (mlib_s16
						    *)(MLIB_GET_POINTER2
						    (zrtable, t, zz));
						data = MLIB_GET_POINTER2(rtable,
						    t, zz);
						xz = (x | zz);
						zdata1[xz] = stz;
						x++;
						stz = z >> MLIB_SHIFT;
						z += dz;
						t += dt;
					}

					MLIB_BLENDING(data[xz], CA);
				} else {
					t = y;
					stz = z >> MLIB_SHIFT;
					z += dz;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (j = 0; j <= n; j++) {
						zz = (zdata[x] - stz) >> 31;
						zdata1 = (mlib_s16
						    *)(MLIB_GET_POINTER2
						    (zrtable, t, zz));
						data = MLIB_GET_POINTER2(rtable,
						    t, zz);
						zdata1[x | zz] = stz;
						MLIB_BLENDING(data[x | zz], CA);
						x++;
						stz = z >> MLIB_SHIFT;
						z += dz;
					}
				}
			} else {
				zdata = (mlib_s16 *)(zrtable[y]);
				data = rtable[-1];
				t = ~x;
				stz = z >> MLIB_SHIFT;
				sdt = t >> MLIB_SHIFT;
				xz = -1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j <= n; j++) {
					zz = (zdata[sdt] - stz) >> 31;
					t += dt;
					z += dz;
					MLIB_BLENDING(data[xz], CA);
					zdata1 = *(mlib_s16 **)((mlib_u8 *)
					    zrtable +
					    MLIB_POINTER_COEF * (y | zz));
					data = *(mlib_u8 **)((mlib_u8 *)rtable +
					    MLIB_POINTER_COEF * (y | zz));
					xz = sdt | zz;
					y++;
					zdata1[xz] = stz;
					sdt = t >> MLIB_SHIFT;
					stz = z >> MLIB_SHIFT;
					zdata = (mlib_s16 *)((mlib_u8 *)zdata +
					    zstride);
				}
				MLIB_BLENDING(data[xz], CA);
			}
		}
	}
}

/* *********************************************************** */

#define	MLIB_DEMUX_CHANNELS(r, g, b, a, s)                      \
	r = ((s) & 0xff);                                       \
	g = ((s) >> 8) & 0xff;                                  \
	b = ((s) >> 16) & 0xff;					\
	a = (mlib_s32)0xff000000;

/* *********************************************************** */

#define	MLIB_MUX_CHANNELS(d, r, g, b, a)                        \
	d = (a | (b << 16) | (g << 8) | r);

/* *********************************************************** */

#define	MLIB_BLENDING_32(d)                                     \
	{                                                       \
	    MLIB_DEMUX_CHANNELS(rs, gs, bs, as, d);             \
	    MLIB_BLENDING(rs, RCA);                             \
	    MLIB_BLENDING(gs, GCA);                             \
	    MLIB_BLENDING(bs, BCA);                             \
	    MLIB_MUX_CHANNELS(d, rs, gs, bs, as);             	\
	}

/* *********************************************************** */

void
mlib_GraphicsSpamLineSet_BZ_32(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 stride,
    mlib_s32 zstride,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 c,
    mlib_s32 a,
    mlib_d64 dc)
{
	mlib_s32 *data;
	mlib_s16 *zdata, *zdata1;
	mlib_s32 j, x, y, z, n, t, dt, dz, zz, stz, sdt, xz;
	mlib_s32 rs, gs, bs, as;
	mlib_s32 rc, gc, bc, ac;
	mlib_d64 RCA, GCA, BCA, A1;

	a &= 0xff;

	MLIB_DEMUX_CHANNELS(rc, gc, bc, ac, c);

	A1 = ((255. - a) / 255.);
	RCA = rc * a / 255.;
	GCA = gc * a / 255.;
	BCA = bc * a / 255.;

	x = stride;
	y = dc;

	for (; coords < coordsend; coords += MLIB_GDS_NEXT_GZ) {
		n = coords[MLIB_GDS_LENGTHLINES];
		y = coords[MLIB_GDS_STARTY];
		x = coords[MLIB_GDS_STARTX];
		z = ~coords[MLIB_GDS_STARTZ];
		dz = coords[MLIB_GDS_STEPZ];
		dt = coords[MLIB_GDS_STEPLINES];

		if (n >= 0) {
			if (y < 0) {
				y = ~y;
				data = (mlib_s32 *)(rtable[y >> MLIB_SHIFT]);
				zdata = (mlib_s16 *)(zrtable[y >> MLIB_SHIFT]);

				if (dt) {
					t = y;
					stz = z >> MLIB_SHIFT;
					z += dz;
					data = (mlib_s32 *)(rtable[-1]);
					xz = -1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (j = 0; j <= n; j++) {
						zdata = (mlib_s16
						    *)(MLIB_GET_POINTER
						    (zrtable, t));
						zz = (zdata[x] - stz) >> 31;
						MLIB_BLENDING_32(data[xz]);
						zdata1 = (mlib_s16
						    *)(MLIB_GET_POINTER2
						    (zrtable, t, zz));
						data = (mlib_s32
						    *)(MLIB_GET_POINTER2
						    (rtable, t, zz));
						xz = (x | zz);
						zdata1[xz] = stz;
						x++;
						stz = z >> MLIB_SHIFT;
						z += dz;
						t += dt;
					}

					MLIB_BLENDING_32(data[xz]);
				} else {
					t = y;
					stz = z >> MLIB_SHIFT;
					z += dz;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (j = 0; j <= n; j++) {
						zz = (zdata[x] - stz) >> 31;
						zdata1 = (mlib_s16
						    *)(MLIB_GET_POINTER2
						    (zrtable, t, zz));
						data = (mlib_s32
						    *)(MLIB_GET_POINTER2
						    (rtable, t, zz));
						zdata1[x | zz] = stz;
						MLIB_BLENDING_32(data[x | zz]);
						x++;
						stz = z >> MLIB_SHIFT;
						z += dz;
					}
				}
			} else {
				zdata = (mlib_s16 *)(zrtable[y]);
				data = (mlib_s32 *)(rtable[-1]);
				t = ~x;
				stz = z >> MLIB_SHIFT;
				sdt = t >> MLIB_SHIFT;
				xz = -1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j <= n; j++) {
					zz = (zdata[sdt] - stz) >> 31;
					t += dt;
					z += dz;
					MLIB_BLENDING_32(data[xz]);
					zdata1 = *(mlib_s16 **)((mlib_u8 *)
					    zrtable +
					    MLIB_POINTER_COEF * (y | zz));
					data = *(mlib_s32 **)((mlib_u8 *)rtable
					    + MLIB_POINTER_COEF * (y | zz));
					xz = sdt | zz;
					y++;
					zdata1[xz] = stz;
					sdt = t >> MLIB_SHIFT;
					stz = z >> MLIB_SHIFT;
					zdata = (mlib_s16 *)((mlib_u8 *)zdata +
					    zstride);
				}

				MLIB_BLENDING_32(data[xz]);
			}
		}
	}
}

/* *********************************************************** */
