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

#pragma ident	"@(#)mlib_v_GraphicsSpamLineSet_G.c	9.2	07/11/05 SMI"

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
#include <vis_proto.h>

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

#define	MLIB_SHIFT_TO_7	8

/* *********************************************************** */

void
mlib_GraphicsSpamLineSet_G_8(
    mlib_u8 **rtable,
    mlib_s32 stride,
    mlib_s32 *coords,
    mlib_s32 *coordsend)
{
	mlib_d64 w_c, w_dc;
	mlib_u8 *data, *data1, *dataend;
	mlib_s32 j, x, y, c, n, t, t2, dt, dc;

	vis_write_gsr(7);

	for (; coords < coordsend; coords += MLIB_GDS_NEXT_GZ) {
		n = coords[MLIB_GDS_LENGTHLINES];
		y = coords[MLIB_GDS_STARTY];
		x = coords[MLIB_GDS_STARTX];
		c = ~coords[MLIB_GDS_STARTC];
		dc = coords[MLIB_GDS_STEPC];
		dt = coords[MLIB_GDS_STEPLINES];

		if (n >= 0)
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

/* The following piece of text is waiting for the better compiler */

/*
 *             n  += x;
 *             t  += dt;
 *             w_c   = vis_to_double(0, c >> (MLIB_SHIFT_TO_7 - 1));
 *             w_c2  = vis_to_double(0, (c + dc) >> (MLIB_SHIFT_TO_7 - 1));
 *             w_dc  = vis_to_double(0, dc >> (MLIB_SHIFT_TO_7 - 2));
 *
 *             for(; x < n; x += 2) {
 *             w_tc       = vis_faligndata(w_c, w_c);
 *             w_c        = vis_fpadd16(w_c, w_dc);
 *             w_tc2      = vis_faligndata(w_c2, w_c2);
 *             w_c2       = vis_fpadd16(w_c2, w_dc);
 *             data1      = MLIB_GET_POINTER(t);
 *             t         += dt;
 *             vis_st_u8_i(w_tc, data, x);
 *             vis_st_u8_i(w_tc2, data1, x + 1);
 *             data       = MLIB_GET_POINTER(t);
 *             t         += dt;
 *             }
 *
 *             if (x > n) continue;
 *             w_tc  = vis_faligndata(w_c, w_c);
 *             vis_st_u8_i(w_tc, data, n);
 */
				} else {
					data += x;
					data1 =
					    (mlib_u8 *)(((mlib_addr)data +
					    3) & ~3);
					dataend = data + n + 1;

					if (data1 <
					    (mlib_u8 *)((mlib_addr)dataend &
					    ~7)) {
						mlib_s32 tc0, tc1, tc2, tc3;

						for (; data < data1; data++) {
							*data = c >> MLIB_SHIFT;
							c += dc;
						}

						data1 =
						    (mlib_u8 *)((mlib_addr)
						    dataend & ~7);

						tc0 =
						    (c << (23 -
						    MLIB_SHIFT)) & ~0xFFFF;
						tc1 =
						    (c + dc) >> MLIB_SHIFT_TO_7;
						tc2 =
						    ((c + 2 * dc) << (23 -
						    MLIB_SHIFT)) & ~0xFFFF;
						tc3 =
						    (c +
						    3 * dc) >> MLIB_SHIFT_TO_7;
						w_c =
						    vis_to_double(tc0 | tc1,
						    tc2 | tc3);
						tc0 =
						    dc >> (MLIB_SHIFT_TO_7 - 2);
						w_dc =
						    vis_to_double_dup((tc0 &
						    0xFFFF) | (tc0 << 16));

						for (; data < data1;
						    data += 4) {
							*(mlib_f32 *)data =
							    vis_fpack16(w_c);
							w_c =
							    vis_fpadd16(w_c,
							    w_dc);
						}

						vis_pst_8(vis_fpack16_pair(w_c,
						    vis_fpadd16(w_c, w_dc)),
						    data,
						    0xFF << (data - dataend +
						    8));
					} else {
						for (; data < dataend; data++) {
							*data = c >> MLIB_SHIFT;
							c += dc;
						}
					}
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

/* *********************************************************** */

#define	MLIB_PUT_G_32(DATA, X)                                  \
	w_c = vis_fpadd16(w_c, w_dc);                           \
	*(mlib_f32 *)((mlib_u8 *)DATA + (X)) = v_t;             \
	v_t = vis_fpack16(w_c)

/* *********************************************************** */

void
mlib_GraphicsSpamLineSet_G_32(
    mlib_u8 **rtable,
    mlib_s32 stride,
    mlib_s32 *coords,
    mlib_s32 *coordsend)
{
	mlib_d64 w_c, w_c2, w_dc, w_dc2;
	mlib_d64 w_const = vis_to_double(0x7FFFFFFF, 0xFFFFFFFF);
	mlib_f32 v_t, v_zero = vis_to_float(0);
	mlib_u8 *data, *data1, *dataend;
	mlib_s32 j, x, y, n, t, dt;

	for (; coords < coordsend; coords += MLIB_GDS_NEXT_GZ32) {
		vis_write_gsr(8 << 3);

		n = coords[MLIB_GDS_LENGTHLINES];
		y = coords[MLIB_GDS_STARTY];
		x = coords[MLIB_GDS_STARTX];
		dt = coords[MLIB_GDS_STEPLINES];

		w_c2 =
		    vis_freg_pair(v_zero,
		    ((mlib_f32 *)coords)[MLIB_GDS_STARTC2]);
		w_c =
		    vis_freg_pair(((mlib_f32 *)coords)[MLIB_GDS_STARTC1],
		    ((mlib_f32 *)coords)[MLIB_GDS_STARTC]);
		w_dc2 =
		    vis_freg_pair(v_zero,
		    ((mlib_f32 *)coords)[MLIB_GDS_STEPC2]);
		w_dc =
		    vis_freg_pair(((mlib_f32 *)coords)[MLIB_GDS_STEPC1],
		    ((mlib_f32 *)coords)[MLIB_GDS_STEPC]);
		w_c2 = vis_fxor(w_c2, w_const);
		w_c = vis_fnot(w_c);

		w_c = vis_fpackfix_pair(w_c2, w_c);
		w_dc = vis_fpackfix_pair(w_dc2, w_dc);

		vis_write_gsr(0);

		v_t = vis_fpack16(w_c);
		vis_fpadd16(w_c, w_dc);

		if (n >= 0)
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
						MLIB_PUT_G_32(data, x);
						data = MLIB_GET_POINTER(t);
						t += dt;
						MLIB_PUT_G_32(data1, x + 4);
					}

					if (x > n)
						continue;
					MLIB_PUT_G_32(data, n);
				} else {
					data += 4 * x;
					dataend = data + 4 * n;
					for (; data <= dataend - 4; data += 8) {
						MLIB_PUT_G_32(data, 0);
						MLIB_PUT_G_32(data, 4);
					}

					if (data <= dataend) {
						MLIB_PUT_G_32(data, 0);
					}
				}
			} else {
				data = rtable[y];
				t = ~x;

				if (dt & (MLIB_ONE - 1)) {
					dataend = rtable[y + n];
					data1 = data + stride;
					for (; data < dataend - 2 * stride; ) {
						MLIB_PUT_G_32(data,
						    ((t >> (MLIB_SHIFT -
						    2)) & ~3));
						t += dt;
						data += 2 * stride;
						MLIB_PUT_G_32(data1,
						    ((t >> (MLIB_SHIFT -
						    2)) & ~3));
						t += dt;
						data1 += 2 * stride;
						MLIB_PUT_G_32(data,
						    ((t >> (MLIB_SHIFT -
						    2)) & ~3));
						t += dt;
						data += 2 * stride;
						MLIB_PUT_G_32(data1,
						    ((t >> (MLIB_SHIFT -
						    2)) & ~3));
						t += dt;
						data1 += 2 * stride;
					}

					if (data > dataend)
						continue;

					if (data < dataend) {
						MLIB_PUT_G_32(data,
						    ((t >> (MLIB_SHIFT -
						    2)) & ~3));
						t += dt;
						data += 2 * stride;
						MLIB_PUT_G_32(data1,
						    ((t >> (MLIB_SHIFT -
						    2)) & ~3));
						t += dt;
						data1 += 2 * stride;
					}

					if (data <= dataend) {
						MLIB_PUT_G_32(data,
						    ((t >> (MLIB_SHIFT -
						    2)) & ~3));
					}
				} else {
					x = (t >> (MLIB_SHIFT - 2)) & ~3;
					t = stride + (dt >> (MLIB_SHIFT - 2));
					data1 = data + t;
					t *= 2;

					for (j = 0; j < n; j += 2) {
						MLIB_PUT_G_32(data, x);
						MLIB_PUT_G_32(data1, x);
						x += t;
					}

					if (j <= n) {
						MLIB_PUT_G_32(data, x);
					}
				}
			}
	}
}

/* *********************************************************** */
