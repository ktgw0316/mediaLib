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

#pragma ident	"@(#)mlib_c_GraphicsSpamLineSet.c	9.2	07/10/09 SMI"

/*
 *  FUNCTIONS
 *    mlib_GraphicsSpamLineSet_8  - line drawing function for mlib_u8 image
 *    mlib_GraphicsSpamLineSet_32 - line drawing function for mlib_s32 image
 *
 *  SYNOPSIS
 *    void  mlib_GraphicsSpamLineSet_8
 *                                    (mlib_u8  **rtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 color,
 *                                     mlib_d64 dcolor);
 *    void  mlib_GraphicsSpamLineSet_32
 *                                    (mlib_u8  **rtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 color,
 *                                     mlib_d64 dcolor);
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
 *   coordsend    for(; coords < coordsend; coords += MLIB_GDS_NEXT)
 *    c           Color to draw, for MLIB_BYTE type of image color must be
 *                  the same in all bytes :
 *                    c = (color & 0xFF),
 *                    c |= (c << 8),
 *                    c |= (c << 16)
 *   dcolor       dcolor = vis_to_double_dup(color)
 */

#include <mlib_graphics.h>
#include <mlib_GraphicsDrawSet.h>

/* *********************************************************** */

#ifdef _LP64

#define	MLIB_GET_POINTER(T)                                         \
	* (mlib_u8 **)((mlib_u8 *)rtable + (((T) >> (MLIB_SHIFT -   \
		3)) & ~7))

#else /* _LP64 */

#define	MLIB_GET_POINTER(T)                                         \
	* (mlib_u8 **)((mlib_u8 *)rtable + (((T) >> (MLIB_SHIFT -   \
		2)) & ~3))
#endif /* _LP64 */

/* *********************************************************** */

void
mlib_GraphicsSpamLineSet_8(
	mlib_u8 **rtable,
	mlib_s32 stride,
	mlib_s32 *coords,
	mlib_s32 *coordsend,
	mlib_s32 c,
	mlib_d64 dc)
{
	mlib_u8 *data, *data1, *dataend;
	mlib_s32 j, x, y, n, t, dt;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; coords < coordsend; coords += MLIB_GDS_NEXT) {
		n = coords[MLIB_GDS_LENGTHLINES];
		y = coords[MLIB_GDS_STARTY];
		x = coords[MLIB_GDS_STARTX];
		dt = coords[MLIB_GDS_STEPLINES];

		if (n >= 0) {
			if (y < 0) {
				t = ~y;
				data = MLIB_GET_POINTER(t);

				if (dt) {
					n += x;
					t += dt;

					for (; x < n; x += 2) {
						data1 = MLIB_GET_POINTER(t);
						t += dt;
						data[x] = c;
						data = MLIB_GET_POINTER(t);
						t += dt;
						data1[x + 1] = c;
					}

					if (x > n)
						continue;
					data[n] = c;
				} else {
					data += x;
					data1 = (mlib_u8 *)(((mlib_addr)data +
						7) & (mlib_addr)(~7));
					dataend = data + n + 1;

					if (data1 < dataend) {
						for (; data < data1; data++)
							*data = c;
						data1 = (mlib_u8 *)((mlib_addr)
							dataend &
							(mlib_addr)(~7));
						for (; data < data1; data += 8)
							*(mlib_d64 *)data = dc;
					}

					for (; data < dataend; data++)
						*data = c;
				}
			} else {
				data = rtable[y];
				t = ~x;
				x = t >> MLIB_SHIFT;

				if (dt & (MLIB_ONE - 1)) {
					dataend = rtable[y + n];
					data1 = data + stride;
					for (; data < dataend - 2 * stride; ) {
						data[t >> MLIB_SHIFT] = c;
						t += dt;
						data += 2 * stride;
						data1[t >> MLIB_SHIFT] = c;
						t += dt;
						data1 += 2 * stride;
						data[t >> MLIB_SHIFT] = c;
						t += dt;
						data += 2 * stride;
						data1[t >> MLIB_SHIFT] = c;
						t += dt;
						data1 += 2 * stride;
					}

					if (data > dataend)
						continue;

					if (data < dataend) {
						data[t >> MLIB_SHIFT] = c;
						t += dt;
						data += 2 * stride;
						data1[t >> MLIB_SHIFT] = c;
						t += dt;
						data1 += 2 * stride;
					}

					if (data <= dataend)
						data[t >> MLIB_SHIFT] = c;
				} else {
					t = stride + (dt >> MLIB_SHIFT);
					for (j = 0; j <= n; j++)
						data[x + j * t] = c;
				}
			}
		}
	}
}

/* *********************************************************** */

#define	MLIB_PUT32(data, T, c)	*(mlib_s32*)((mlib_u8*)data + (T)) = c
#define	MLIB_INC32(data, inc)	data = (mlib_s32*)((mlib_u8*)data + (inc))

#define	COND	(mlib_u8 *)data < (mlib_u8 *)dataend - 2 * stride

/* *********************************************************** */

void
mlib_GraphicsSpamLineSet_32(
	mlib_u8 **rtable,
	mlib_s32 stride,
	mlib_s32 *coords,
	mlib_s32 *coordsend,
	mlib_s32 c,
	mlib_d64 dc)
{
	mlib_s32 *data, *data1, *dataend;
	mlib_s32 j, x, y, n, t, dt;

	for (; coords < coordsend; coords += MLIB_GDS_NEXT) {
		n = coords[MLIB_GDS_LENGTHLINES];
		y = coords[MLIB_GDS_STARTY];
		x = coords[MLIB_GDS_STARTX];
		dt = coords[MLIB_GDS_STEPLINES];

		if (n >= 0) {
			if (y < 0) {
				t = ~y;
				data = (mlib_s32 *)MLIB_GET_POINTER(t);

				if (dt) {
					n += x;
					t += dt;
					x <<= 2;
					n <<= 2;

					for (; x < n; x += 8) {
						data1 = (mlib_s32 *)
							MLIB_GET_POINTER(t);
						t += dt;
						MLIB_PUT32(data, x, c);
						data = (mlib_s32 *)
							MLIB_GET_POINTER(t);
						t += dt;
						MLIB_PUT32(data1, (x + 4), c);
					}

					if (x > n)
						continue;
					MLIB_PUT32(data, n, c);
				} else {
					data += x;
					data1 = (mlib_s32 *)(((mlib_addr)data +
						7) & (mlib_addr)(~7));
					dataend = data + n + 1;

					if (data1 < dataend) {
						if (data < data1)
							(*data++) = c;
						data1 = (mlib_s32 *)((mlib_addr)
							dataend &
							(mlib_addr)(~7));
						for (; data < data1; data += 2)
							*(mlib_d64 *)data = dc;
					}

					if (data < dataend)
						*data = c;
				}
			} else {
				data = (mlib_s32 *)rtable[y];
				t = ~x;

				if (dt & (MLIB_ONE - 1)) {
					dataend = (mlib_s32 *)(rtable[y + n]);
					data1 = (mlib_s32 *)((mlib_u8 *)data +
						stride);
					for (; COND; ) {
						MLIB_PUT32(data,
							(t >> (MLIB_SHIFT -
							2)) & ~3, c);
						t += dt;
						MLIB_INC32(data, 2 * stride);
						MLIB_PUT32(data1,
							(t >> (MLIB_SHIFT -
							2)) & ~3, c);
						t += dt;
						MLIB_INC32(data1, 2 * stride);
						MLIB_PUT32(data,
							(t >> (MLIB_SHIFT -
							2)) & ~3, c);
						t += dt;
						MLIB_INC32(data, 2 * stride);
						MLIB_PUT32(data1,
							(t >> (MLIB_SHIFT -
							2)) & ~3, c);
						t += dt;
						MLIB_INC32(data1, 2 * stride);
					}

					if (data > dataend)
						continue;

					if (data < dataend) {
						MLIB_PUT32(data,
							(t >> (MLIB_SHIFT -
							2)) & ~3, c);
						t += dt;
						MLIB_INC32(data, 2 * stride);
						MLIB_PUT32(data1,
							(t >> (MLIB_SHIFT -
							2)) & ~3, c);
						t += dt;
						MLIB_INC32(data1, 2 * stride);
					}

					if (data <= dataend)
						MLIB_PUT32(data,
							(t >> (MLIB_SHIFT -
							2)) & ~3, c);
				} else {
					x = (t >> (MLIB_SHIFT - 2)) & ~3;
					t = stride + (dt >> (MLIB_SHIFT - 2));
					for (j = 0; j <= n; j++)
						MLIB_PUT32(data, x + j * t, c);
				}
			}
		}
	}
}

/* *********************************************************** */

/* unoptimized version */
/*
 *  mlib_u8 *data;
 *  mlib_s32 j, x, y, n, t, dt;
 *
 *  for(;coords < coordsend; coords += MLIB_GDS_NEXT) {
 *    n  = coords[MLIB_GDS_LENGTHLINES];
 *    y  = coords[MLIB_GDS_STARTY];
 *    x  = coords[MLIB_GDS_STARTX];
 *    dt = coords[MLIB_GDS_STEPLINES];
 *
 *    if (n >= 0)
 *    if (y < 0) {
 *      y = ~y;
 *      data = rtable[y >> MLIB_SHIFT];
 *
 *      if (dt) {
 *        t = y;
 *        for(j = 0; j <= n; j++) {
 *          data = *(mlib_u8**)((mlib_u8*)rtable +
 *            (((t + j * dt) >> (MLIB_SHIFT - 2)) &~0x3));
 *          data[x + j] = c;
 *        }
 *
 *      } else {
 *        for(j = 0; j <= n; j++) data[x + j] = c;
 *      }
 *
 *    } else {
 *      data = rtable[y];
 *      t = ~x;
 *      x = t >> MLIB_SHIFT;
 *
 *      if (dt & (MLIB_ONE - 1)) {
 *        for(j = 0; j <= n; j++)
 *          data[j * stride + ((t + j * dt) >> MLIB_SHIFT)] = c;
 *      } else {
 *        t     = stride + (dt >> MLIB_SHIFT);
 *        for(j = 0; j <= n; j++) data[x + j * t] = c;
 *      }
 *    }
 *  }
 *
 */

/* *********************************************************** */
