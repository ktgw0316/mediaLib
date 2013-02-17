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

#pragma ident	"@(#)mlib_v_GraphicsSpamLineSet_X.c	9.2	07/11/05 SMI"

/*
 *  FUNCTIONS
 *    mlib_GraphicsSpamLineSet_X_8  - line drawing function for mlib_u8 image
 *      with XOR mode
 *    mlib_GraphicsSpamLineSet_X_32 - line drawing function for mlib_s32 image
 *      with XOR mode
 *
 *  SYNOPSIS
 *    void  mlib_GraphicsSpamLineSet_X_8
 *                                    (mlib_u8  **rtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 color,
 *                                     mlib_d64 dcolor);
 *    void  mlib_GraphicsSpamLineSet_X_32
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
 *    c           Color to xor, for MLIB_BYTE type of image color must be
 *                  the same in all bytes :
 *                    c = (color & 0xFF),
 *                    c |= (c << 8),
 *                    c |= (c << 16)
 *   dcolor       dcolor = vis_to_double_dup(color)
 */

#include <mlib_graphics.h>
#include <mlib_GraphicsDrawSet.h>
#include <vis_proto.h>

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

#define	MLIB_GET(T)	*(mlib_u8**)((mlib_u8*)rtable + T)

/* *********************************************************** */

#define	MLIB_FILL_ROW_X_8(prow, xb, xe, dcolor)                            \
	{                                                                  \
	    mlib_s32 _i, _w = (xe) - (xb);                                 \
	    mlib_u8 *_pdst_row = (prow) + (xb);                            \
	    mlib_u8 *_pdst_end = _pdst_row + _w;                           \
	    mlib_s32 _mask = vis_edge8(_pdst_row, _pdst_end);              \
	    mlib_d64 *_dpdst_row = vis_alignaddr(_pdst_row, 0), _ddata =   \
		*_dpdst_row;                                               \
	    _ddata = vis_fxor(_ddata, dcolor);                             \
	    if (_w >= 0)                                                   \
		vis_pst_8(_ddata, _dpdst_row, _mask);                      \
	    _dpdst_row++;                                                  \
	    _i = (mlib_addr)_dpdst_row - (mlib_addr)_pdst_row;             \
	    for (; _i <= (_w - 7); _i += 8) {                              \
		_ddata = *_dpdst_row;                                      \
		_ddata = vis_fxor(_ddata, dcolor);                         \
		(*_dpdst_row++) = _ddata;                                  \
	    }                                                              \
	    if (_i <= _w) {                                                \
		_mask = vis_edge8(_dpdst_row, _pdst_end);                  \
		_ddata = *_dpdst_row;                                      \
		_ddata = vis_fxor(_ddata, dcolor);                         \
		vis_pst_8(_ddata, _dpdst_row, _mask);                      \
	    }                                                              \
	}

/* *********************************************************** */

void
mlib_GraphicsSpamLineSet_X_8(
    mlib_u8 **rtable,
    mlib_s32 stride,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 c,
    mlib_d64 dc)
{
	mlib_u8 *data, *data1, *data2, *data3, *dataend;
	mlib_s32 j, x, y, n, t, dt;

#pragma pipeloop(0)
	for (; coords < coordsend; coords += MLIB_GDS_NEXT) {
		n = coords[MLIB_GDS_LENGTHLINES];
		y = coords[MLIB_GDS_STARTY];
		x = coords[MLIB_GDS_STARTX];
		dt = coords[MLIB_GDS_STEPLINES];

		if (n >= 0)
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

#pragma pipeloop(0)
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
						data[x] ^= c;
						data = MLIB_GET(std);
						std = MLIB_SHIFT_POINTER(t);
						t += dt;
						data1[x + 1] ^= c;
						data2[x + 2] ^= c;
						data3[x + 3] ^= c;
					}

					if (x > n)
						continue;

					if (x < n) {
						data[x] ^= c;
						data1 = MLIB_GET(std1);
						data = MLIB_GET(std2);
						data1[x + 1] ^= c;
						x += 2;
					}

					if (x <= n)
						data[x] ^= c;
				} else {
					mlib_d64 *wd, wx;
					mlib_s32 emask;

					data += x;
					dataend = data + n;
					emask = vis_edge8(data, dataend);
					wd = vis_alignaddr(data, 0);
					wx = *wd;
					wx = vis_fxor(wx, dc);
					vis_pst_8(wx, wd, emask);
					wd++;
					j = (mlib_addr)wd - (mlib_addr)data;
#pragma pipeloop(0)
					for (; j <= (n - 7); j += 8) {
						wx = *wd;
						wx = vis_fxor(wx, dc);
						(*wd++) = wx;
					}

					if (j <= n) {
						emask = vis_edge8(wd, dataend);
						wx = *wd;
						wx = vis_fxor(wx, dc);
						vis_pst_8(wx, wd, emask);
					}
				}
			} else {
				data = rtable[y];
				t = ~x;
				x = t >> MLIB_SHIFT;

				if (dt & (MLIB_ONE - 1)) {
					dataend = rtable[y + n];
					data1 = data + stride;
#pragma pipeloop(0)
					for (; data < dataend - 2 * stride; ) {
						data[t >> MLIB_SHIFT] ^= c;
						t += dt;
						data += 2 * stride;
						data1[t >> MLIB_SHIFT] ^= c;
						t += dt;
						data1 += 2 * stride;
						data[t >> MLIB_SHIFT] ^= c;
						t += dt;
						data += 2 * stride;
						data1[t >> MLIB_SHIFT] ^= c;
						t += dt;
						data1 += 2 * stride;
					}

					if (data > dataend)
						continue;

					if (data < dataend) {
						data[t >> MLIB_SHIFT] ^= c;
						t += dt;
						data += 2 * stride;
						data1[t >> MLIB_SHIFT] ^= c;
						t += dt;
						data1 += 2 * stride;
					}

					if (data <= dataend)
						data[t >> MLIB_SHIFT] ^= c;
				} else {
					t = stride + (dt >> MLIB_SHIFT);
#pragma pipeloop(0)
					for (j = 0; j <= n; j++)
						data[x + j * t] ^= c;
				}
			}
	}
}

/* *********************************************************** */

#define	MLIB_XOR32(data, T, c)	*(mlib_s32*)((mlib_u8*)data + (T)) ^= c
#define	MLIB_INC32(data, inc)	data = (mlib_s32*)((mlib_u8*)data + (inc))
#define	COND	(mlib_u8 *)data < (mlib_u8 *)dataend - 2 * stride

/* *********************************************************** */

void
mlib_GraphicsSpamLineSet_X_32(
    mlib_u8 **rtable,
    mlib_s32 stride,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 c,
    mlib_d64 dc)
{
	mlib_s32 *data, *data1, *data2, *data3, *dataend;
	mlib_s32 j, x, y, n, t, dt;

#pragma pipeloop(0)
	for (; coords < coordsend; coords += MLIB_GDS_NEXT) {
		n = coords[MLIB_GDS_LENGTHLINES];
		y = coords[MLIB_GDS_STARTY];
		x = coords[MLIB_GDS_STARTX];
		dt = coords[MLIB_GDS_STEPLINES];

		if (n >= 0)
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

#pragma pipeloop(0)
					for (; x < n - 2; x += 4) {
						data1 =
						    (mlib_s32 *)MLIB_GET(std1);
						data2 =
						    (mlib_s32 *)MLIB_GET(std2);
						data3 =
						    (mlib_s32 *)MLIB_GET(std3);
						std1 = MLIB_SHIFT_POINTER(t);
						t += dt;
						std2 = MLIB_SHIFT_POINTER(t);
						t += dt;
						std3 = MLIB_SHIFT_POINTER(t);
						t += dt;
						data[x] ^= c;
						data =
						    (mlib_s32 *)MLIB_GET(std);
						std = MLIB_SHIFT_POINTER(t);
						t += dt;
						data1[x + 1] ^= c;
						data2[x + 2] ^= c;
						data3[x + 3] ^= c;
					}

					if (x > n)
						continue;

					if (x < n) {
						data[x] ^= c;
						data1 =
						    (mlib_s32 *)MLIB_GET(std1);
						data =
						    (mlib_s32 *)MLIB_GET(std2);
						data1[x + 1] ^= c;
						x += 2;
					}

					if (x <= n)
						data[x] ^= c;
				} else {
					mlib_d64 wx;

					data += x;
					j = 0;

					if ((mlib_addr)data & 4)
						data[j++] ^= c;

#pragma pipeloop(0)
					for (; j <= n - 1; j += 2) {
						wx = *((mlib_d64 *)(data + j));
						wx = vis_fxor(wx, dc);
						*((mlib_d64 *)(data + j)) = wx;
					}

					if (j <= n)
						data[j] ^= c;
				}
			} else {
				data = (mlib_s32 *)rtable[y];
				t = ~x;
				x = t >> MLIB_SHIFT;

				if (dt & (MLIB_ONE - 1)) {
					dataend = (mlib_s32 *)rtable[y + n];
					data1 = data;
					MLIB_INC32(data1, stride);

#pragma pipeloop(0)
					for (; COND; ) {
						MLIB_XOR32(data,
						    (t >> (MLIB_SHIFT -
						    2)) & ~3, c);
						t += dt;
						MLIB_INC32(data, 2 * stride);
						MLIB_XOR32(data1,
						    (t >> (MLIB_SHIFT -
						    2)) & ~3, c);
						t += dt;
						MLIB_INC32(data1, 2 * stride);
						MLIB_XOR32(data,
						    (t >> (MLIB_SHIFT -
						    2)) & ~3, c);
						t += dt;
						MLIB_INC32(data, 2 * stride);
						MLIB_XOR32(data1,
						    (t >> (MLIB_SHIFT -
						    2)) & ~3, c);
						t += dt;
						MLIB_INC32(data1, 2 * stride);
					}

					if (data > dataend)
						continue;

					if (data < dataend) {
						MLIB_XOR32(data,
						    (t >> (MLIB_SHIFT -
						    2)) & ~3, c);
						t += dt;
						MLIB_INC32(data, 2 * stride);
						MLIB_XOR32(data1,
						    (t >> (MLIB_SHIFT -
						    2)) & ~3, c);
						t += dt;
						MLIB_INC32(data1, 2 * stride);
					}

					if (data <= dataend)
						MLIB_XOR32(data,
						    (t >> (MLIB_SHIFT -
						    2)) & ~3, c);
				} else {
					t = stride + (dt >> (MLIB_SHIFT - 2));
					data += x;

#pragma pipeloop(0)
					for (j = 0; j <= n; j++)
						MLIB_XOR32(data, j * t, c);
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
 *          data[x + j] ^= c;
 *        }
 *
 *      } else {
 *        for(j = 0; j <= n; j++) data[x + j] ^= c;
 *      }
 *
 *    } else {
 *      data = rtable[y];
 *      t = -x;
 *      x = t >> MLIB_SHIFT;
 *
 *      if (dt & (MLIB_ONE - 1)) {
 *        for(j = 0; j <= n; j++)
 *          data[j * stride + ((t + j * dt) >> MLIB_SHIFT)] ^= c;
 *      } else {
 *        t     = stride + (dt >> MLIB_SHIFT);
 *        for(j = 0; j <= n; j++) data[x + j * t] ^= c;
 *      }
 *    }
 *  }
 *
 */

/* *********************************************************** */
