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

#pragma ident	"@(#)mlib_c_GraphicsSpamLineSet_Z.c	9.2	07/10/09 SMI"

/*
 *  FUNCTIONS
 *    mlib_GraphicsSpamLineSet_Z_8  - line drawing function
 *                                    with Z - buffer mode
 *    mlib_GraphicsSpamLineSet_Z_32 - line drawing function
 *                                    with Z - buffer mode
 *
 *  SYNOPSIS
 *    void  mlib_GraphicsSpamLineSet_Z_8
 *                                    (mlib_u8  **rtable,
 *                                     mlib_u8  **zrtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 zstride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 color,
 *                                     mlib_d64 dcolor);
 *    void  mlib_GraphicsSpamLineSet_Z_32
 *                                    (mlib_u8  **rtable,
 *                                     mlib_u8  **zrtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 zstride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 color,
 *                                     mlib_d64 dcolor);
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
 *   color        Color to draw, for MLIB_BYTE type of image color must be
 *                  the same in all bytes :
 *                    c = (color & 0xFF),
 *                    c |= (c << 8),
 *                    c |= (c << 16)
 */

#include <mlib_graphics.h>
#include <mlib_GraphicsDrawSet.h>

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

void
mlib_GraphicsSpamLineSet_Z_8(
	mlib_u8 **rtable,
	mlib_u8 **zrtable,
	mlib_s32 stride,
	mlib_s32 zstride,
	mlib_s32 *coords,
	mlib_s32 *coordsend,
	mlib_s32 c,
	mlib_d64 dc)
{
	mlib_u8 *data;
	mlib_s16 *zdata, *zdata1;
	mlib_s32 j, x, y, z, n, t, dt, dz, zz, stz, sdt, xz;

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
						data[xz] = c;
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

					data[xz] = c;
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
						data[x | zz] = c;
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
					data[xz] = c;
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

				data[xz] = c;
			}
		}
	}
}

/* *********************************************************** */

void
mlib_GraphicsSpamLineSet_Z_32(
	mlib_u8 **rtable,
	mlib_u8 **zrtable,
	mlib_s32 stride,
	mlib_s32 zstride,
	mlib_s32 *coords,
	mlib_s32 *coordsend,
	mlib_s32 c,
	mlib_d64 dc)
{
	mlib_s32 *data;
	mlib_s16 *zdata, *zdata1;
	mlib_s32 j, x, y, z, n, t, dt, dz, zz, stz, sdt, xz;

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
						data[xz] = c;
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

					data[xz] = c;
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
						data[x | zz] = c;
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
					data[xz] = c;
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

				data[xz] = c;
			}
		}
	}
}

/* *********************************************************** */

/* unoptimized version */
/*
 *  mlib_u8  *data;
 *  mlib_s16 *zdata;
 *  mlib_s32 j, x, y, z, n, t, dt, dz;
 *
 *  for(;coords < coordsend; coords += MLIB_GDS_NEXT_GZ) {
 *    n  = coords[MLIB_GDS_LENGTHLINES];
 *    y  = coords[MLIB_GDS_STARTY];
 *    x  = coords[MLIB_GDS_STARTX];
 *    z  = ~coords[MLIB_GDS_STARTZ];
 *    dz = coords[MLIB_GDS_STEPZ];
 *    dt = coords[MLIB_GDS_STEPLINES];
 *
 *    if (n >= 0)
 *    if (y < 0) {
 *      y = ~y;
 *      data  = rtable[y >> MLIB_SHIFT];
 *      zdata = (mlib_s16*)(zrtable[y >> MLIB_SHIFT]);
 *
 *      if (dt) {
 *        t = y;
 *        for(j = 0; j <= n; j++) {
 *          data  = MLIB_GET_POINTER(rtable, t + j * dt);
 *          zdata = (mlib_s16*)(MLIB_GET_POINTER(zrtable, t + j * dt));
 *
 *          if (zdata[x + j] >= (z >> MLIB_SHIFT)) {
 *            data[x + j]  = c;
 *            zdata[x + j] = (z >> MLIB_SHIFT);
 *          }
 *
 *          z += dz;
 *        }
 *
 *      } else {
 *        for(j = 0; j <= n; j++) {
 *          if (zdata[x + j] >= (z >> MLIB_SHIFT)) {
 *            data[x + j] = c;
 *            zdata[x + j] = (z >> MLIB_SHIFT);
 *          }
 *
 *          z += dz;
 *        }
 *      }
 *
 *    } else {
 *      data  = rtable[y];
 *      zdata = (mlib_s16*)(zrtable[y]);
 *      t = ~x;
 *      x = t >> MLIB_SHIFT;
 *
 *      if (dt & (MLIB_ONE - 1)) {
 *        for(j = 0; j <= n; j++) {
 *          if (zdata[j * zstride / 2 + ((t + j * dt) >> MLIB_SHIFT)] >=
 *              (z >> MLIB_SHIFT)) {
 *            zdata[j * zstride / 2 + ((t + j * dt) >> MLIB_SHIFT)] =
 *            z >> MLIB_SHIFT;
 *            data[j * stride + ((t + j * dt) >> MLIB_SHIFT)] = c;
 *          }
 *
 *          z += dz;
 *        }
 *
 *      } else {
 *        t     = stride + (dt >> MLIB_SHIFT);
 *        dt    = zstride + (dt >> (MLIB_SHIFT - 1));
 *        for(j = 0; j <= n; j++) {
 *          if (zdata[x + j*dt / 2] >= (z >> MLIB_SHIFT)) {
 *            zdata[x + j*dt / 2] = z >> MLIB_SHIFT;
 *            data[x + j*t] = c;
 *          }
 *
 *          z += dz;
 *        }
 *      }
 *    }
 *  }
 *
 */

/* *********************************************************** */
