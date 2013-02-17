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

#pragma ident	"@(#)mlib_c_GraphicsSpamLineSet_G.c	9.3	07/10/09 SMI"

/*
 *  FUNCTIONS
 *    mlib_GraphicsSpamLineSet_G_8  - line drawing function for mlib_u8 image
 *                                      with Gouraud mode
 *    mlib_GraphicsSpamLineSet_G_32 - line drawing function for mlib_s32 image
 *                                      with Gouraud mode
 *
 *  SYNOPSIS
 *    void  mlib_GraphicsSpamLineSet_G_8
 *                                    (mlib_u8  **rtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend);
 *    void  mlib_GraphicsSpamLineSet_G_32
 *                                    (mlib_u8  **rtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend);
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
mlib_GraphicsSpamLineSet_G_8(
	mlib_u8 **rtable,
	mlib_s32 stride,
	mlib_s32 *coords,
	mlib_s32 *coordsend)
{
	mlib_u8 *data, *data1, *dataend;
	mlib_s32 j, x, y, c, c_, n, t, t2, dt, dc;

	for (; coords < coordsend; coords += MLIB_GDS_NEXT_GZ) {
		n = coords[MLIB_GDS_LENGTHLINES];
		y = coords[MLIB_GDS_STARTY];
		x = coords[MLIB_GDS_STARTX];
		c = ~coords[MLIB_GDS_STARTC];
		dc = coords[MLIB_GDS_STEPC];
		dt = coords[MLIB_GDS_STEPLINES];

		if (n >= 0) {
			if (y < 0) {
				t = ~y;
				data = rtable[t >> MLIB_SHIFT];

				if (dt) {
					n += x;
					t += dt;

					for (; x < n; x += 2) {
						data1 = MLIB_GET_POINTER(t);
						t += dt;
						data[x] = c >> MLIB_SHIFT;
						c += dc;
						data = MLIB_GET_POINTER(t);
						t += dt;
						data1[x + 1] = c >> MLIB_SHIFT;
						c += dc;
					}

					if (x > n)
						continue;
					data[n] = c >> MLIB_SHIFT;
				} else {
					data += x;
					dataend = data + n;
					c_ = c + dc;
					dc *= 2;

					for (; data <= dataend - 3; data += 4) {
						data[0] = c >> MLIB_SHIFT;
						c += dc;
						data[1] = c_ >> MLIB_SHIFT;
						c_ += dc;
						data[2] = c >> MLIB_SHIFT;
						c += dc;
						data[3] = c_ >> MLIB_SHIFT;
						c_ += dc;
					}

					if (data < dataend) {
						data[0] = c >> MLIB_SHIFT;
						c += dc;
						data[1] = c_ >> MLIB_SHIFT;
						data += 2;
					}

					if (data <= dataend)
						data[0] = c >> MLIB_SHIFT;
				}
			} else {
				data = rtable[y];
				t = ~x;
				x = t >> MLIB_SHIFT;

				if (dt & (MLIB_ONE - 1)) {
					dataend = rtable[y + n];
					data1 = data + stride;
					for (; data < dataend - 2 * stride; ) {
						data[t >> MLIB_SHIFT] =
							c >> MLIB_SHIFT;
						t += dt;
						data += 2 * stride;
						c += dc;
						data1[t >> MLIB_SHIFT] =
							c >> MLIB_SHIFT;
						t += dt;
						data1 += 2 * stride;
						c += dc;
						data[t >> MLIB_SHIFT] =
							c >> MLIB_SHIFT;
						t += dt;
						data += 2 * stride;
						c += dc;
						data1[t >> MLIB_SHIFT] =
							c >> MLIB_SHIFT;
						t += dt;
						data1 += 2 * stride;
						c += dc;
					}

					if (data > dataend)
						continue;

					if (data < dataend) {
						data[t >> MLIB_SHIFT] =
							c >> MLIB_SHIFT;
						t += dt;
						data += 2 * stride;
						c += dc;
						data1[t >> MLIB_SHIFT] =
							c >> MLIB_SHIFT;
						t += dt;
						data1 += 2 * stride;
						c += dc;
					}

					if (data <= dataend)
						data[t >> MLIB_SHIFT] =
							c >> MLIB_SHIFT;
				} else {
					t = stride + (dt >> MLIB_SHIFT);
					data += x;
					t2 = 2 * t;

					for (j = 0; j < n; j += 2) {
						data[0] = c >> MLIB_SHIFT;
						c += dc;
						data[t] = c >> MLIB_SHIFT;
						c += dc;
						data += t2;
					}

					if (j <= n)
						data[0] = c >> MLIB_SHIFT;
				}
			}
		}
	}
}

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN

#define	MLIB_PUT_G_32(DATA, DATAFROM, X)                        \
	DATA = DATAFROM + (X);                                  \
	DATA[3] = c2 >> (MLIB_SHIFT + 8);                       \
	DATA[2] = c2 >> MLIB_SHIFT;                             \
	c2 += dc2;                                              \
	DATA[1] = c1 >> MLIB_SHIFT;                             \
	c1 += dc1;                                              \
	DATA[0] = c >> MLIB_SHIFT;                              \
	c += dc

#else /* _LITTLE_ENDIAN */

#define	MLIB_PUT_G_32(DATA, DATAFROM, X)                        \
	DATA = DATAFROM + (X);                                  \
	*(mlib_s16 *)DATA = c2 >> MLIB_SHIFT;                   \
	c2 += dc2;                                              \
	DATA[2] = c1 >> MLIB_SHIFT;                             \
	c1 += dc1;                                              \
	DATA[3] = c >> MLIB_SHIFT;                              \
	c += dc;
#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

void
mlib_GraphicsSpamLineSet_G_32(
	mlib_u8 **rtable,
	mlib_s32 stride,
	mlib_s32 *coords,
	mlib_s32 *coordsend)
{
	mlib_u8 *data, *data1, *data2, *dataend;
	mlib_s32 j, x, y, c, c1, c2, n, t, dt, dc, dc1, dc2;

	for (; coords < coordsend; coords += MLIB_GDS_NEXT_GZ32) {
		n = coords[MLIB_GDS_LENGTHLINES];
		y = coords[MLIB_GDS_STARTY];
		x = coords[MLIB_GDS_STARTX];
		dt = coords[MLIB_GDS_STEPLINES];
		c2 = (~coords[MLIB_GDS_STARTC2]) | (0xFF << (MLIB_SHIFT + 8));
		dc2 = coords[MLIB_GDS_STEPC2];
		c1 = ~coords[MLIB_GDS_STARTC1];
		dc1 = coords[MLIB_GDS_STEPC1];
		c = ~coords[MLIB_GDS_STARTC];
		dc = coords[MLIB_GDS_STEPC];

		if (n >= 0) {
			if (y < 0) {
				t = ~y;
				data = rtable[t >> MLIB_SHIFT];

				if (dt) {
					n += x;
					n *= 4;
					x *= 4;
					t += dt;

					for (; x < n; x += 8) {
						data1 = MLIB_GET_POINTER(t);
						t += dt;
						MLIB_PUT_G_32(data, data, x);
						data = MLIB_GET_POINTER(t);
						t += dt;
						MLIB_PUT_G_32(data1, data1,
							x + 4);
					}

					if (x > n)
						continue;
					MLIB_PUT_G_32(data, data, n);
				} else {
					data += 4 * x;
					dataend = data + 4 * n;
					for (; data <= dataend - 4; data += 8) {
#ifdef _LITTLE_ENDIAN
						data[3] =
							c2 >> (MLIB_SHIFT + 8);
						data[2] = c2 >> MLIB_SHIFT;
						c2 += dc2;
						data[1] = c1 >> MLIB_SHIFT;
						c1 += dc1;
						data[0] = c >> MLIB_SHIFT;
						c += dc;
						data[7] =
							c2 >> (MLIB_SHIFT + 8);
						data[6] = c2 >> MLIB_SHIFT;
						c2 += dc2;
						data[5] = c1 >> MLIB_SHIFT;
						c1 += dc1;
						data[4] = c >> MLIB_SHIFT;
						c += dc;
#else /* _LITTLE_ENDIAN */
						*(mlib_s16 *)data =
							c2 >> MLIB_SHIFT;
						c2 += dc2;
						data[2] = c1 >> MLIB_SHIFT;
						c1 += dc1;
						data[3] = c >> MLIB_SHIFT;
						c += dc;
						*(mlib_s16 *)(data + 4) =
							c2 >> MLIB_SHIFT;
						c2 += dc2;
						data[6] = c1 >> MLIB_SHIFT;
						c1 += dc1;
						data[7] = c >> MLIB_SHIFT;
						c += dc;
#endif /* _LITTLE_ENDIAN */
					}

					if (data <= dataend) {
#ifdef _LITTLE_ENDIAN
						data[3] =
							c2 >> (MLIB_SHIFT + 8);
						data[2] = c2 >> MLIB_SHIFT;
						c2 += dc2;
						data[1] = c1 >> MLIB_SHIFT;
						c1 += dc1;
						data[0] = c >> MLIB_SHIFT;
						c += dc;
#else /* _LITTLE_ENDIAN */
						*(mlib_s16 *)data =
							c2 >> MLIB_SHIFT;
						c2 += dc2;
						data[2] = c1 >> MLIB_SHIFT;
						c1 += dc1;
						data[3] = c >> MLIB_SHIFT;
						c += dc;
#endif /* _LITTLE_ENDIAN */
					}
				}
			} else {
				data = rtable[y];
				t = ~x;

				if (dt & (MLIB_ONE - 1)) {
					dataend = rtable[y + n];
					data1 = data + stride;
					for (; data < dataend - 2 * stride; ) {
						MLIB_PUT_G_32(data2, data,
							((t >> (MLIB_SHIFT -
							2)) & ~3));
						t += dt;
						data += 2 * stride;
						MLIB_PUT_G_32(data2, data1,
							((t >> (MLIB_SHIFT -
							2)) & ~3));
						t += dt;
						data1 += 2 * stride;
						MLIB_PUT_G_32(data2, data,
							((t >> (MLIB_SHIFT -
							2)) & ~3));
						t += dt;
						data += 2 * stride;
						MLIB_PUT_G_32(data2, data1,
							((t >> (MLIB_SHIFT -
							2)) & ~3));
						t += dt;
						data1 += 2 * stride;
					}

					if (data > dataend)
						continue;

					if (data < dataend) {
						MLIB_PUT_G_32(data2, data,
							((t >> (MLIB_SHIFT -
							2)) & ~3));
						t += dt;
						data += 2 * stride;
						MLIB_PUT_G_32(data2, data1,
							((t >> (MLIB_SHIFT -
							2)) & ~3));
						t += dt;
						data1 += 2 * stride;
					}

					if (data <= dataend) {
						MLIB_PUT_G_32(data2, data,
							((t >> (MLIB_SHIFT -
							2)) & ~3));
					}
				} else {
					x = (t >> (MLIB_SHIFT - 2)) & ~3;
					t = stride + (dt >> (MLIB_SHIFT - 2));
					for (j = 0; j <= n; j++) {
						MLIB_PUT_G_32(data1, data,
							x + j * t);
					}
				}
			}
		}
	}
}

/* *********************************************************** */

/* unoptimized version */

/*
 *  mlib_u8  *data;
 *  mlib_s32 j, x, y, c, n, t, dt, dc;
 *
 *  for(;coords < coordsend; coords += MLIB_GDS_NEXT_GZ) {
 *    n  = coords[MLIB_GDS_LENGTHLINES];
 *    y  = coords[MLIB_GDS_STARTY];
 *    x  = coords[MLIB_GDS_STARTX];
 *    c  = ~coords[MLIB_GDS_STARTC];
 *    dc = coords[MLIB_GDS_STEPC];
 *    dt = coords[MLIB_GDS_STEPLINES];
 *
 *    if (n >= 0)
 *    if (y < 0) {
 *      y = ~y;
 *      data  = rtable[y >> MLIB_SHIFT];
 *
 *      if (dt) {
 *        t = y;
 *        for(j = 0; j <= n; j++) {
 *          data  = MLIB_GET_POINTER(t + j * dt);
 *          data[x + j]  = (c + j * dc) >> MLIB_SHIFT;
 *        }
 *
 *      } else {
 *        for(j = 0; j <= n; j++) {
 *            data[x + j] = (c + j * dc) >> MLIB_SHIFT;
 *        }
 *      }
 *
 *    } else {
 *      data  = rtable[y];
 *      t = ~x;
 *      x = t >> MLIB_SHIFT;
 *
 *      if (dt & (MLIB_ONE - 1)) {
 *        for(j = 0; j <= n; j++) {
 *            data[j * stride + ((t + j * dt) >> MLIB_SHIFT)] =
 *            (c + j * dc) >> MLIB_SHIFT;
 *        }
 *
 *      } else {
 *        t     = stride + (dt >> MLIB_SHIFT);
 *        for(j = 0; j <= n; j++) {
 *            data[x + j*t] = (c + j * dc) >> MLIB_SHIFT;
 *        }
 *      }
 *    }
 *  }
 *
 */

/* unoptimized 32-bit version */

/*
 *  mlib_u8  *data, *data1;
 *  mlib_s32 j, x, y, c, c1, c2, n, t, dt, dc, dc1, dc2;
 *
 *  for(;coords < coordsend; coords += MLIB_GDS_NEXT_GZ32) {
 *    n   = coords[MLIB_GDS_LENGTHLINES];
 *    y   = coords[MLIB_GDS_STARTY];
 *    x   = coords[MLIB_GDS_STARTX];
 *    c   = ~coords[MLIB_GDS_STARTC];
 *    c1  = ~coords[MLIB_GDS_STARTC1];
 *    c2  = ~coords[MLIB_GDS_STARTC2];
 *    dc  = coords[MLIB_GDS_STEPC];
 *    dc1 = coords[MLIB_GDS_STEPC1];
 *    dc2 = coords[MLIB_GDS_STEPC2];
 *    dt  = coords[MLIB_GDS_STEPLINES];
 *
 *    if (n >= 0)
 *    if (y < 0) {
 *      y = ~y;
 *      data  = rtable[y >> MLIB_SHIFT];
 *
 *      if (dt) {
 *        t = y;
 *        for(j = 0; j <= n; j++) {
 *          data          = MLIB_GET_POINTER(t + j * dt);
 *          data1         = data + 4 * (x + j);
 *          data1[0]      = 0xFF;
 *          data1[1]      = (c2 + j * dc2) >> MLIB_SHIFT;
 *          data1[2]      = (c1 + j * dc1) >> MLIB_SHIFT;
 *          data1[3]      = (c + j * dc) >> MLIB_SHIFT;
 *        }
 *
 *      } else {
 *        for(j = 0; j <= n; j++) {
 *          data1         = data + 4 * (x + j);
 *          data1[0]      = 0xFF;
 *          data1[1]      = (c2 + j * dc2) >> MLIB_SHIFT;
 *          data1[2]      = (c1 + j * dc1) >> MLIB_SHIFT;
 *          data1[3]      = (c + j * dc) >> MLIB_SHIFT;
 *        }
 *      }
 *
 *    } else {
 *      data  = rtable[y];
 *      t = ~x;
 *
 *      if (dt & (MLIB_ONE - 1)) {
 *        for(j = 0; j <= n; j++) {
 *          data1      = data + (((t + j * dt) >> (MLIB_SHIFT - 2)) &~ 0x3);
 *          data1[0]   = 0xFF;
 *          data1[1]   = (c2 + j * dc2) >> MLIB_SHIFT;
 *          data1[2]   = (c1 + j * dc1) >> MLIB_SHIFT;
 *          data1[3]   = (c + j * dc) >> MLIB_SHIFT;
 *          data      += stride;
 *        }
 *
 *      } else {
 *        x     = t >> MLIB_SHIFT;
 *        t     = stride + ((dt >> (MLIB_SHIFT - 2)) &~ 0x3);
 *        for(j = 0; j <= n; j++) {
 *          data1      = data + 4 * x + j * t;
 *          data1[0]   = 0xFF;
 *          data1[1]   = (c2 + j * dc2) >> MLIB_SHIFT;
 *          data1[2]   = (c1 + j * dc1) >> MLIB_SHIFT;
 *          data1[3]   = (c + j * dc) >> MLIB_SHIFT;
 *        }
 *      }
 *    }
 *  }
 *
 */

/* *********************************************************** */
