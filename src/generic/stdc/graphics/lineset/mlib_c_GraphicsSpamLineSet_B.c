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

#pragma ident	"@(#)mlib_c_GraphicsSpamLineSet_B.c	9.2	07/10/09 SMI"

/*
 *  FUNCTIONS
 *    mlib_GraphicsSpamLineSet_B_8  - line drawing function for mlib_u8 image
 *      with alpha blending
 *    mlib_GraphicsSpamLineSet_B_32 - line drawing function for mlib_s32 image
 *      with alpha blending
 *
 *  SYNOPSIS
 *    void  mlib_GraphicsSpamLineSet_B_8
 *                                    (mlib_u8  **rtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_d64 ca,
 *                                     mlib_d64 a_1,
 *                                     mlib_d64 vca,
 *                                     mlib_f32 va_1);
 *    void  mlib_GraphicsSpamLineSet_B_32
 *                                    (mlib_u8  **rtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_d64 ca1,
 *                                     mlib_d64 ca2,
 *                                     mlib_d64 ca3,
 *                                     mlib_d64 a_1,
 *                                     mlib_d64 vca,
 *                                     mlib_d64 va_1);
 *
 *
 * ARGUMENTS
 *    rtable      Pointer to array of starts of row, rtable[-1][-1] is buffer
 *                  for unwanted stores
 *    stride      Stride of the image
 *    coords      Array of values sets used in *Spam* functions:
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
 *   ca*          (color * alpha / 255.) as mlib_d64 value.
 *   a_1          (alpha / 255. - 1) as mlib_d64 value.
 *   vca, va_1    Used in VIS version only.
 *
 * DESCRIPTION
 *    The drawable buffer is either MLIB_BYTE (8-bit) or MLIB_INT (32-bit)
 *    type single channel mediaLib image.
 *
 *    Pixels falling outside the drawable buffer will be clipped and discarded.
 *
 *    All colors are specified in 32-bit format. For 8-bit buffer, only
 *    the bits 0-7 are used and the bits 8-23 should be zero.
 *
 *    In alpha value, only the bits 0-7 are used and the bits 8-23 should
 *    be zero.  For each drawable pixel, the original pixel value is
 *    blended with the drawing color to produce the final pixel value
 *
 *        d = (s * (255 - a) + c * a) / 255
 *
 *    where c is the color used in the drawing, a is the alpha value, s
 *    is the original value of the pixel, and d is the final value of the
 *    pixel.
 */

#include <mlib_graphics.h>
#include <mlib_GraphicsDrawSet.h>
#include <mlib_GraphicsBlend.h>

/* *********************************************************** */

#ifdef _LP64

#define	MLIB_GET_POINTER(T)                                         \
	* (mlib_u8 **)((mlib_u8 *)rtable + (((T) >> (MLIB_SHIFT -   \
		3)) & ~7))

#define	MLIB_SHIFT_POINTER(T)	(((T) >> (MLIB_SHIFT - 3)) &~ 7)

#else /* _LP64 */

#define	MLIB_GET_POINTER(T)                                         \
	* (mlib_u8 **)((mlib_u8 *)rtable + (((T) >> (MLIB_SHIFT -   \
		2)) & ~3))

#define	MLIB_SHIFT_POINTER(T)	(((T) >> (MLIB_SHIFT - 2)) &~ 3)
#endif /* _LP64 */

/* *********************************************************** */

#define	MLIB_GET(T)	*(mlib_u8**)((mlib_u8*)rtable + T)

/* *********************************************************** */

#define	BLEND_8	MLIB_GRAPHICS_BLEND_8
#define	BLEND_8_4 MLIB_GRAPHICS_BLEND_8_4

#define	UNROLL_FAC 4

void
mlib_GraphicsSpamLineSet_B_8(
	mlib_u8 **rtable,
	mlib_s32 stride,
	mlib_s32 *coords,
	mlib_s32 *coordsend,
	mlib_d64 ca,
	mlib_d64 a_1,
	mlib_d64 vca,
	mlib_f32 va_1)
{
	mlib_u8 *data, *data1, *data2, *data3, *dataend;
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
					mlib_s32 std, std1, std2, std3;

					n += x;
					t += dt;
					std1 = MLIB_SHIFT_POINTER(t);
					t += dt;
					std2 = MLIB_SHIFT_POINTER(t);
					t += dt;
					std3 = MLIB_SHIFT_POINTER(t);
					t += dt;
					std = MLIB_SHIFT_POINTER(t);
					t += dt;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (; x < n - 2; x += 4) {
						data1 = MLIB_GET(std1);
						data2 = MLIB_GET(std2);
						data3 = MLIB_GET(std3);
						std1 = MLIB_SHIFT_POINTER(t);
						t += dt;
						std2 = MLIB_SHIFT_POINTER(t);
						t += dt;
						std3 = MLIB_SHIFT_POINTER(t);
						t += dt;
						BLEND_8(data + x)
						data = MLIB_GET(std);
						std = MLIB_SHIFT_POINTER(t);
						t += dt;
						BLEND_8(data1 + x + 1)
						BLEND_8(data2 + x + 2)
						BLEND_8(data3 + x + 3)
					}

					if (x > n)
						continue;

					if (x < n) {
						BLEND_8(data + x)
						data1 = MLIB_GET(std1);
						data = MLIB_GET(std2);
						BLEND_8(data1 + x + 1)
						x += 2;
					}

					if (x <= n)
						BLEND_8(data + x)
				} else {
					data += x;
					data1 = (mlib_u8 *)(((mlib_addr)data +
						7) & (mlib_addr)(~7));
					dataend = data + n + 1;

					if (data1 < dataend) {
						for (; data < data1; data++)
							BLEND_8(data)
						data1 = (mlib_u8 *)((mlib_addr)
							dataend &
							(mlib_addr)(~7));
#ifdef __SUNPRO_C
#pragma unroll(UNROLL_FAC)
#endif /* __SUNPRO_C */
						for (; data < data1; data += 4)
							BLEND_8_4((mlib_s32 *)
									data)
					}

					for (; data < dataend; data++)
						BLEND_8(data)
				}
			} else {
				data = rtable[y];
				t = ~x;
				x = t >> MLIB_SHIFT;

				if (dt & (MLIB_ONE - 1)) {
					dataend = rtable[y + n];
					data1 = data + stride;
#ifdef __SUNPRO_C
#pragma unroll(UNROLL_FAC)
#endif /* __SUNPRO_C */
					for (; data < dataend - 2 * stride; ) {
						BLEND_8(data +
							(t >> MLIB_SHIFT))
						t += dt;
						data += 2 * stride;
						BLEND_8(data1 +
							(t >> MLIB_SHIFT))
						t += dt;
						data1 += 2 * stride;
						BLEND_8(data +
							(t >> MLIB_SHIFT))
						t += dt;
						data += 2 * stride;
						BLEND_8(data1 +
							(t >> MLIB_SHIFT))
						t += dt;
						data1 += 2 * stride;
					}

					if (data > dataend)
						continue;

					if (data < dataend) {
						BLEND_8(data +
							(t >> MLIB_SHIFT))
						t += dt;
						data += 2 * stride;
						BLEND_8(data1 +
							(t >> MLIB_SHIFT))
						t += dt;
						data1 += 2 * stride;
					}

					if (data <= dataend)
						BLEND_8(data +
							(t >> MLIB_SHIFT))
				} else {
					t = stride + (dt >> MLIB_SHIFT);
#ifdef __SUNPRO_C
#pragma unroll(UNROLL_FAC)
#endif /* __SUNPRO_C */
					for (j = 0; j <= n; j++)
						BLEND_8(data + x + j * t)
				}
			}
		}
	}
}

/* *********************************************************** */

#define	BLEND_32	MLIB_GRAPHICS_BLEND_32

#define	MLIB_BLEND32(data, T)	\
	BLEND_32((mlib_s32*)((mlib_u8*)data + (T)))

#define	MLIB_INC32(data, inc)	data = (mlib_s32*)((mlib_u8*)data + (inc))

#define	COND	 (mlib_u8 *)data < (mlib_u8 *)dataend - 2 * stride

/* *********************************************************** */

void
mlib_GraphicsSpamLineSet_B_32(
	mlib_u8 **rtable,
	mlib_s32 stride,
	mlib_s32 *coords,
	mlib_s32 *coordsend,
	mlib_d64 ca1,
	mlib_d64 ca2,
	mlib_d64 ca3,
	mlib_d64 a_1,
	mlib_d64 vca,
	mlib_d64 va_1)
{
	mlib_s32 *data, *data1, *data2, *data3, *dataend;
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
				data = (mlib_s32 *)MLIB_GET_POINTER(t);

				if (dt) {
					mlib_s32 std, std1, std2, std3;

					n += x;
					t += dt;
					std1 = MLIB_SHIFT_POINTER(t);
					t += dt;
					std2 = MLIB_SHIFT_POINTER(t);
					t += dt;
					std3 = MLIB_SHIFT_POINTER(t);
					t += dt;
					std = MLIB_SHIFT_POINTER(t);
					t += dt;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (; x < n - 2; x += 4) {
						data1 = (mlib_s32 *)
							MLIB_GET(std1);
						data2 = (mlib_s32 *)
							MLIB_GET(std2);
						data3 = (mlib_s32 *)
							MLIB_GET(std3);
						std1 = MLIB_SHIFT_POINTER(t);
						t += dt;
						std2 = MLIB_SHIFT_POINTER(t);
						t += dt;
						std3 = MLIB_SHIFT_POINTER(t);
						t += dt;
						BLEND_32(data + x)
						data = (mlib_s32 *)
							MLIB_GET(std);
						std = MLIB_SHIFT_POINTER(t);
						t += dt;
						BLEND_32(data1 + x + 1)
						BLEND_32(data2 + x + 2)
						BLEND_32(data3 + x + 3)
					}

					if (x > n)
						continue;

					if (x < n) {
						BLEND_32(data + x)
						data1 = (mlib_s32 *)
							MLIB_GET(std1);
						data = (mlib_s32 *)
							MLIB_GET(std2);
						BLEND_32(data1 + x + 1)
						x += 2;
					}

					if (x <= n)
						BLEND_32(data + x)
				} else {
					data += x;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (j = 0; j <= n; j++)
						BLEND_32(data + j)
				}
			} else {
				data = (mlib_s32 *)rtable[y];
				t = ~x;
				x = t >> MLIB_SHIFT;

				if (dt & (MLIB_ONE - 1)) {
					dataend = (mlib_s32 *)rtable[y + n];
					data1 = data;
					MLIB_INC32(data1, stride);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (; COND; ) {
						MLIB_BLEND32(data,
							(t >> (MLIB_SHIFT -
							2)) & ~3);
						t += dt;
						MLIB_INC32(data, 2 * stride);
						MLIB_BLEND32(data1,
							(t >> (MLIB_SHIFT -
							2)) & ~3);
						t += dt;
						MLIB_INC32(data1, 2 * stride);
						MLIB_BLEND32(data,
							(t >> (MLIB_SHIFT -
							2)) & ~3);
						t += dt;
						MLIB_INC32(data, 2 * stride);
						MLIB_BLEND32(data1,
							(t >> (MLIB_SHIFT -
							2)) & ~3);
						t += dt;
						MLIB_INC32(data1, 2 * stride);
					}

					if (data > dataend)
						continue;

					if (data < dataend) {
						MLIB_BLEND32(data,
							(t >> (MLIB_SHIFT -
							2)) & ~3);
						t += dt;
						MLIB_INC32(data, 2 * stride);
						MLIB_BLEND32(data1,
							(t >> (MLIB_SHIFT -
							2)) & ~3);
						t += dt;
						MLIB_INC32(data1, 2 * stride);
					}

					if (data <= dataend)
						MLIB_BLEND32(data,
							(t >> (MLIB_SHIFT -
							2)) & ~3);
				} else {
					t = stride + (dt >> (MLIB_SHIFT - 2));
					data += x;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (j = 0; j <= n; j++)
						MLIB_BLEND32(data, j * t);
				}
			}
		}
	}
}

/* *********************************************************** */

/* unoptimized version */
/*
 *  mlib_s32 *data;
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
 *      y = -y;
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
 *      t = -x;
 *      x = t >> MLIB_SHIFT;
 *
 *      if (dt & (MLIB_ONE - 1)) {
 *        for(j = 0; j <= n; j++)
 *          data[j * stride + ((t + j * dt) >> MLIB_SHIFT)] = c;
 *      } else {
 *        t     = stride + (dt >> MLIB_SHIFT);
 *        for(j = 0; j <= n; j++) data[x + j*t] = c;
 *      }
 *    }
 *  }
 *
 */

/* *********************************************************** */
