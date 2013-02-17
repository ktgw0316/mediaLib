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

#pragma ident	"@(#)mlib_c_GraphicsSpamLineSet_BGZ.c	9.2	07/10/09 SMI"

/*
 *  FUNCTIONS
 *    mlib_GraphicsSpamLineSet_BGZ_8  - line drawing function
 *	with Gouraud shading,  Z - buffering and alpha blending
 *    mlib_GraphicsSpamLineSet_BGZ_32 - line drawing function
 *	with Gouraud shading,  Z - buffering and alpha blending
 *
 *  SYNOPSIS
 *    void  mlib_GraphicsSpamLineSet_BGZ_8
 *                                    (mlib_u8  **rtable,
 *                                     mlib_u8  **zrtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 zstride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *				       mlib_s32 alpha);
 *    void  mlib_GraphicsSpamLineSet_BGZ_32
 *                                    (mlib_u8  **rtable,
 *                                     mlib_u8  **zrtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 zstride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *				       mlib_s32 alpha);
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
 *   alpha        Alpha value for the blending.
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

#else /* _LP64 */

#define	MLIB_GET_POINTER(RT, T)                                            \
	* (mlib_u8 **)((mlib_u8 *)RT + (((T) >> (MLIB_SHIFT - 2)) & ~3))

#define	MLIB_GET_POINTER2(RT, T, ZZ)                             \
	* (mlib_u8 **)((mlib_u8 *)RT + ((((T) >> (MLIB_SHIFT -   \
		2)) | ZZ) & ~3))

#define	MLIB_POINTER_COEF	4
#endif /* _LP64 */

/* *********************************************************** */

#define	MLIB_BLENDING_8(s, c)                       	\
    s = (mlib_U82D64[s] * A0) + (mlib_U82D64[c] * A1)

/* *********************************************************** */

void
mlib_GraphicsSpamLineSet_BGZ_8(
	mlib_u8 **rtable,
	mlib_u8 **zrtable,
	mlib_s32 stride,
	mlib_s32 zstride,
	mlib_s32 *coords,
	mlib_s32 *coordsend,
	mlib_s32 a)
{
	mlib_u8 *data;
	mlib_s16 *zdata, *zdata1;
	mlib_s32 j, x, y, z, c, n, t, dt, dz, dc, zz, stz, st, stc, xz;
	mlib_d64 A0, A1;

	A1 = (a & 0xFF) / 255.;
	A0 = 1. - A1;

	x = stride;

	for (; coords < coordsend; coords += MLIB_GDS_NEXT_GZ) {
		n = coords[MLIB_GDS_LENGTHLINES];
		y = coords[MLIB_GDS_STARTY];
		x = coords[MLIB_GDS_STARTX];
		z = ~coords[MLIB_GDS_STARTZ];
		dz = coords[MLIB_GDS_STEPZ];
		c = ~coords[MLIB_GDS_STARTC];
		dc = coords[MLIB_GDS_STEPC];
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
					stc = c >> MLIB_SHIFT;
					data = rtable[-1];
					xz = -1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (j = 0; j <= n; j++) {
						zdata = (mlib_s16 *)
						(MLIB_GET_POINTER(zrtable, t));
						zz = (zdata[x] - stz) >> 31;
						MLIB_BLENDING_8(data[xz], stc);
						stc = c >> MLIB_SHIFT;
						c += dc;
						zdata1 = (mlib_s16 *)
						(MLIB_GET_POINTER2(zrtable,
						t, zz));
						data =
						MLIB_GET_POINTER2
						(rtable, t, zz);
						xz = (x | zz);
						zdata1[xz] = stz;
						x++;
						stz = z >> MLIB_SHIFT;
						z += dz;
						t += dt;
					}

					MLIB_BLENDING_8(data[xz], stc);
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
						MLIB_BLENDING_8(data[x | zz],
						(c >> MLIB_SHIFT));
						x++;
						c += dc;
						stz = z >> MLIB_SHIFT;
						z += dz;
					}
				}
			} else {
				zdata = (mlib_s16 *)(zrtable[y]);
				data = rtable[-1];
				t = ~x;
				stc = c >> MLIB_SHIFT;
				stz = z >> MLIB_SHIFT;
				st = t >> MLIB_SHIFT;
				xz = -1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j <= n; j++) {
					zz = (zdata[st] - stz) >> 31;
					t += dt;
					z += dz;
					MLIB_BLENDING_8(data[xz], stc);
					zdata1 = *(mlib_s16 **)((mlib_u8 *)
						zrtable +
						MLIB_POINTER_COEF * (y | zz));
					data = *(mlib_u8 **)((mlib_u8 *)rtable +
						MLIB_POINTER_COEF * (y | zz));
					xz = st | zz;
					y++;
					zdata1[xz] = stz;
					stc = c >> MLIB_SHIFT;
					c += dc;
					st = t >> MLIB_SHIFT;
					stz = z >> MLIB_SHIFT;
					zdata = (mlib_s16 *)((mlib_u8 *)zdata +
						zstride);
				}

				MLIB_BLENDING_8(data[xz], stc);
			}
		}
	}
}

/* *********************************************************** */

#define	MLIB_PUT_BG_32(DATA, DATAFROM, X)                              \
	DATA = DATAFROM + (X);                                         \
	src = *((mlib_s32 *)DATA);                                     \
	b = (src >> 16) & 0xFF;                                        \
	g = (src >> 8) & 0xFF;                                         \
	r = src & 0xFF;                                                \
	MLIB_BLENDING_8(b, (c2 >> MLIB_SHIFT) & 0xFF);                 \
	MLIB_BLENDING_8(g, (c1 >> MLIB_SHIFT));                        \
	MLIB_BLENDING_8(r, (c >> MLIB_SHIFT));                         \
	*((mlib_s32 *)DATA) = 0xFF000000 | (b << 16 | (g << 8) | r);   \

/* *********************************************************** */

#define	MLIB_LOAD_32                                                     \
	z = ~coords[MLIB_GDS_STARTZ];                                    \
	dz = coords[MLIB_GDS_STEPZ];                                     \
	c2 = (~coords[MLIB_GDS_STARTC2]) | (0xFF << (MLIB_SHIFT + 8));   \
	dc2 = coords[MLIB_GDS_STEPC2];                                   \
	c1 = ~coords[MLIB_GDS_STARTC1];                                  \
	dc1 = coords[MLIB_GDS_STEPC1];                                   \
	c = ~coords[MLIB_GDS_STARTC];                                    \
	dc = coords[MLIB_GDS_STEPC]

/* *********************************************************** */

void
mlib_GraphicsSpamLineSet_BGZ_32(
	mlib_u8 **rtable,
	mlib_u8 **zrtable,
	mlib_s32 stride,
	mlib_s32 zstride,
	mlib_s32 *coords,
	mlib_s32 *coordsend,
	mlib_s32 a)
{
	mlib_u8 *data, *data1;
	mlib_s16 *zdata, *zdata1;
	mlib_s32 j, x, y, z, n, t, dt, dz, zz, stz, xz;
	mlib_s32 c, c1, c2, dc, dc1, dc2, r, g, b, src;
	mlib_d64 A0, A1;

	A1 = a / 255.;
	A0 = 1. - A1;

	x = stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; coords < coordsend; coords += MLIB_GDS_NEXT_GZ32) {
		n = coords[MLIB_GDS_LENGTHLINES];
		y = coords[MLIB_GDS_STARTY];
		x = coords[MLIB_GDS_STARTX];
		dt = coords[MLIB_GDS_STEPLINES];

		if (n >= 0) {
			if (y < 0) {
				t = ~y;

				if (dt) {
					MLIB_LOAD_32;
					n += x;
					x *= 2;
					n *= 2;
					stz = z >> MLIB_SHIFT;
					zdata = (mlib_s16
						*)(MLIB_GET_POINTER(zrtable,
						t));
					zz = (*(mlib_s16 *)((mlib_u8 *)zdata +
						x) - stz) >> 31;
					zdata1 = (mlib_s16 *)
						MLIB_GET_POINTER2(zrtable, t,
						zz);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (; x < n; ) {
						data = MLIB_GET_POINTER2(rtable,
							t, zz);
						t += dt;
						xz = x | (2 * zz);
						x += 2;
						*(mlib_s16 *)((mlib_u8 *)zdata1
							+ xz) = stz;
						z += dz;
						stz = z >> MLIB_SHIFT;
						zdata = (mlib_s16
							*)(MLIB_GET_POINTER
							(zrtable, t));
						zz = (*(mlib_s16 *)((mlib_u8 *)
							zdata + x) - stz) >> 31;
						MLIB_PUT_BG_32(data1,
								data, 2 * xz);

						zdata1 = (mlib_s16 *)
							MLIB_GET_POINTER2
							(zrtable, t, zz);
						c2 += dc2;
						c1 += dc1;
						c += dc;
					}

					data = MLIB_GET_POINTER2(rtable, t, zz);
					xz = x | (2 * zz);
					*(mlib_s16 *)((mlib_u8 *)zdata1 + xz) =
						stz;
					MLIB_PUT_BG_32(data1, data, 2 * xz);
				} else {
					MLIB_LOAD_32;
					zdata = (mlib_s16 *)(zrtable[t >>
						MLIB_SHIFT]);
					stz = z >> MLIB_SHIFT;
					z += dz;
					x *= 2;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (j = 0; j <= n; j++) {
						zz = (*(mlib_s16 *)((mlib_u8 *)
							zdata + x) - stz) >> 31;
						xz = x | (2 * zz);
						zdata1 = (mlib_s16
							*)(MLIB_GET_POINTER2
							(zrtable, t, zz));
						data = MLIB_GET_POINTER2(rtable,
							t, zz);
						*(mlib_s16 *)((mlib_u8 *)zdata1
							+ xz) = stz;
						stz = z >> MLIB_SHIFT;
						z += dz;
						MLIB_PUT_BG_32(data1, data,
							2 * xz);
						x += 2;
						c2 += dc2;
						c1 += dc1;
						c += dc;
					}
				}
			} else {
				MLIB_LOAD_32;
				data = rtable[y];
				zdata = (mlib_s16 *)(zrtable[y]);
				t = ~x;
				x = t >> MLIB_SHIFT;
				stz = z >> MLIB_SHIFT;
				z += dz;
				t += dt;
				x *= 2;

#ifdef __SUNPRO_C
#pragma unroll(1)
#endif /* __SUNPRO_C */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j <= n; j++) {
					zz = (*(mlib_s16 *)((mlib_u8 *)zdata +
						x) - stz) >> 31;
					xz = x | (2 * zz);
					x = (t >> (MLIB_SHIFT - 1)) & ~1;
					t += dt;
					zdata = (mlib_s16 *)((mlib_u8 *)zdata +
						zstride);
					zdata1 = (mlib_s16 *)(zrtable[y | zz]);
					data = rtable[y | zz];
					y++;
					*(mlib_s16 *)((mlib_u8 *)zdata1 + xz) =
						stz;
					stz = z >> MLIB_SHIFT;
					z += dz;
					MLIB_PUT_BG_32(data1, data, 2 * xz);
					c2 += dc2;
					c1 += dc1;
					c += dc;
				}
			}
		}
	}
}

/* *********************************************************** */
