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

#pragma ident	"@(#)mlib_v_GraphicsSpamLineSet_A.c	9.2	07/11/05 SMI"

/*
 *  FUNCTIONS
 *    mlib_GraphicsSpamLineSet_A_8  - line drawing function for mlib_u8 image
 *    mlib_GraphicsSpamLineSet_A_32 - line drawing function for mlib_s32 image
 *
 *  SYNOPSIS
 *    void  mlib_GraphicsSpamLineSet_A_8
 *                                    (mlib_u8  **rtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 color,
 *                                     mlib_d64 dcolor,
 *                                     mlib_s32 width,
 *                                     mlib_s32 height);
 *    void  mlib_GraphicsSpamLineSet_A_32
 *                                    (mlib_u8  **rtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 color,
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
 *    dcolor      dcolor = vis_to_double_dup(color)
 *    width       width = Image.Width - 2, is using to discover
 *                to clip neighbour in *Spam_A* functions
 *    height      height = Image.Height - 2, is using to discover
 *                to clip neighbour in *Spam_A* functions
 */

#include <mlib_graphics.h>
#include <vis_proto.h>
#include <mlib_GraphicsDrawSet.h>

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

#define	MLIB_LOAD_ALPHA8                                                   \
	w_div = vis_ld_u8_i(table, offset);                                \
	w_udiv = vis_ld_u8_i(atable, offset ^ 0xFF);                       \
	w_ddiv = vis_ld_u8_i(atable, offset);                              \
	w_div = vis_fpadd16(w_div, w_div);                                 \
	w_div = vis_fpsub16(w_div, w_01);                                  \
	w_udiv = vis_fpadd16(w_udiv, w_udiv);                              \
	w_ddiv = vis_fpadd16(w_ddiv, w_ddiv);                              \
	w_resc = vis_fmul8x16al(vis_read_lo(w_c), vis_read_lo(w_div));     \
	w_uresc = vis_fmul8x16al(vis_read_lo(w_c), vis_read_lo(w_udiv));   \
	w_dresc = vis_fmul8x16al(vis_read_lo(w_c), vis_read_lo(w_ddiv));   \
	w_div = vis_fpsub16(w_neg, w_div);                                 \
	w_udiv = vis_fpsub16(w_neg, w_udiv);                               \
	w_ddiv = vis_fpsub16(w_neg, w_ddiv)

/* *********************************************************** */

#define	MLIB_MAKE_ALPHA8                                                   \
	w_bg = vis_ld_u8_i(data, x);                                       \
	w_ubg = vis_ld_u8_i(udata, ux);                                    \
	w_dbg = vis_ld_u8_i(ddata, dx);                                    \
	w_resbg = vis_fmul8x16al(vis_read_lo(w_bg), vis_read_lo(w_div));   \
	w_uresbg =                                                         \
	vis_fmul8x16al(vis_read_lo(w_ubg), vis_read_lo(w_udiv));           \
	w_dresbg =                                                         \
	vis_fmul8x16al(vis_read_lo(w_dbg), vis_read_lo(w_ddiv));           \
	w_resbg = vis_fpadd16(w_resc, w_resbg);                            \
	w_uresbg = vis_fpadd16(w_uresc, w_uresbg);                         \
	w_dresbg = vis_fpadd16(w_dresc, w_dresbg);                         \
	vis_st_u8_i(w_resbg, data, x);                                     \
	vis_st_u8_i(w_uresbg, udata, ux);                                  \
	vis_st_u8_i(w_dresbg, ddata, dx)

/* *********************************************************** */

#define	MLIB_MAKE_HOR8(DATA, X)                                          \
	{                                                                \
	    w_bg = vis_ld_u8_i(DATA, X);                                 \
	    w_resbg =                                                    \
		vis_fmul8x16al(vis_read_lo(w_bg), vis_read_lo(w_div));   \
	    w_resbg = vis_fpadd16(w_resc, w_resbg);                      \
	    vis_st_u8_i(w_resbg, DATA, X);                               \
	}

/* *********************************************************** */

#define	MLIB_MAKE_HOR32(DATA, X)                                  \
	{                                                         \
	    mlib_f32 v_bg;                                        \
	                                                          \
	    v_bg = *(mlib_f32 *)(DATA + X);                       \
	    w_resbg = vis_fmul8x16al(v_bg, vis_read_lo(w_div));   \
	    w_resbg = vis_fpadd16(w_resc, w_resbg);               \
	    *(mlib_f32 *)(DATA + X) = vis_fpack16(w_resbg);       \
	}

/* *********************************************************** */

void
mlib_GraphicsSpamLineSet_A_8(
    mlib_u8 **rtable,
    mlib_s32 stride,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 c,
    mlib_d64 dc,
    mlib_s32 width,
    mlib_s32 height)
{
	mlib_u8 *atable, *table, *udata, *data, *ddata;
	mlib_s32 j, x, y, n, t, dt, offset, zz, ux, dx;
	mlib_d64 w_div = 0., w_udiv = 0., w_ddiv = 0.;
	mlib_d64 w_bg = 0., w_ubg = 0., w_dbg = 0.;
	mlib_d64 w_c = dc, w_neg = vis_to_double_dup(255);
	mlib_d64 w_01 = vis_to_double_dup(0x01);
	mlib_d64 w_resc, w_uresc, w_dresc, w_resbg, w_uresbg, w_dresbg;

	c &= 0xFF;

	vis_write_gsr(7 << 3);

#pragma unroll(1)
	for (; coords < coordsend; coords += MLIB_GDS_NEXT) {
		n = coords[MLIB_GDS_LENGTHLINES];
		y = coords[MLIB_GDS_STARTY];
		x = coords[MLIB_GDS_STARTX];
		dt = coords[MLIB_GDS_STEPLINES];
		zz = dt >> 31;
		zz = (dt ^ zz) - zz;
		atable =
		    mlib_GraphicsAliasTab_u8 +
		    512 * mlib_GraphicsSqrtTab_u8[zz >> (MLIB_SHIFT - 7)];
		table = atable + 256;

		if (n >= 0)
			if (y < 0) {
				t = ~y;
				data = rtable[y = (t >> MLIB_SHIFT)];

				if (dt) {
					offset = (t >> (MLIB_SHIFT - 8)) & 0xFF;

#pragma pipeloop(0)
					for (j = 0; j <= n; j++, x++) {
						MLIB_LOAD_ALPHA8;
						y = t >> MLIB_SHIFT;
						t += dt;
						data = rtable[y];
						udata = rtable[y - 1];
						ddata = rtable[y + 1];
						ux = ((y - 1) >> 31) | x;
						dx = ((height - y) >> 31) | x;
						MLIB_MAKE_ALPHA8;
						offset =
						    (t >> (MLIB_SHIFT -
						    8)) & 0xFF;
					}
				} else {
					w_div =
					    vis_to_double_dup(2 *
					    MLIB_BG_COEFF);
					w_resc =
					    vis_fmul8x16al(vis_read_lo(w_c),
					    vis_read_lo(w_div));
					w_div = vis_fpsub16(w_neg, w_div);
					n++;

					if (y > 0) {
						data -= stride;
						data += x;
						udata =
						    (mlib_u8 *)(((mlib_addr)data
						    + 3) & ~3);
						ddata = data + n;

						if (udata < ddata) {
							for (; data < udata;
							    data++)
								MLIB_MAKE_HOR8
								    (data, 0);
							udata =
							    (mlib_u8
							    *)((mlib_addr)ddata
							    & ~3);
#pragma pipeloop(0)
							for (; data < udata;
							    data += 4)
								MLIB_MAKE_HOR32
								    (data, 0);
						}

						for (; data < ddata; data++)
							MLIB_MAKE_HOR8(data, 0);
						data = rtable[y];
					}

					if (y <= height) {
						data += stride;
						data += x;
						udata =
						    (mlib_u8 *)(((mlib_addr)data
						    + 3) & ~3);
						ddata = data + n;

						if (udata < ddata) {
							for (; data < udata;
							    data++)
								MLIB_MAKE_HOR8
								    (data, 0);
							udata =
							    (mlib_u8
							    *)((mlib_addr)ddata
							    & ~3);
#pragma pipeloop(0)
							for (; data < udata;
							    data += 4)
								MLIB_MAKE_HOR32
								    (data, 0);
						}

						for (; data < ddata; data++)
							MLIB_MAKE_HOR8(data, 0);
						data = rtable[y];
					}

					data += x;
					udata =
					    (mlib_u8 *)(((mlib_addr)data +
					    7) & ~7);
					ddata = data + n;

					if (udata < ddata) {
						for (; data < udata; data++)
							*data = c;
						udata =
						    (mlib_u8 *)((mlib_addr)ddata
						    & ~7);
						for (; data < udata; data += 8)
							*(mlib_d64 *)data = dc;
					}

					for (; data < ddata; data++)
						*data = c;
				}
			} else {
				data = rtable[y];
				t = ~x;
				x = t >> MLIB_SHIFT;

				if (dt & (MLIB_ONE - 1)) {

#pragma pipeloop(0)
					for (j = 0; j <= n; j++, y++) {
						offset =
						    (t >> (MLIB_SHIFT -
						    8)) & 0xFF;
						MLIB_LOAD_ALPHA8;
						x = t >> MLIB_SHIFT;
						ux = x - 1;
						dx = (width - x) >> 31;
						t += dt;
						udata = rtable[y | (ux >> 31)];
						ddata = rtable[y | dx];
						dx = (x + 1) | dx;
						MLIB_MAKE_ALPHA8;
						data += stride;
					}
				} else {
					dt = dt >> MLIB_SHIFT;
					w_udiv = vis_ld_u8_i(atable, 128);
					w_udiv = vis_fpadd16(w_udiv, w_udiv);
					w_uresc =
					    vis_fmul8x16al(vis_read_lo(w_c),
					    vis_read_lo(w_udiv));
					w_udiv = vis_fpsub16(w_neg, w_udiv);

#pragma pipeloop(0)
					for (j = 0; j <= n; j++, y++, x += dt) {
						ux = x - 1;
						dx = (width - x) >> 31;
						udata = rtable[y | (ux >> 31)];
						ddata = rtable[y | dx];
						dx = (x + 1) | dx;
						w_ubg = vis_ld_u8_i(udata, ux);
						w_dbg = vis_ld_u8_i(ddata, dx);
						w_uresbg =
						    vis_fmul8x16al(vis_read_lo
						    (w_ubg),
						    vis_read_lo(w_udiv));
						w_dresbg =
						    vis_fmul8x16al(vis_read_lo
						    (w_dbg),
						    vis_read_lo(w_udiv));
						w_uresbg =
						    vis_fpadd16(w_uresc,
						    w_uresbg);
						w_dresbg =
						    vis_fpadd16(w_uresc,
						    w_dresbg);
						vis_st_u8_i(w_c, data, x);
						vis_st_u8_i(w_uresbg, udata,
						    ux);
						vis_st_u8_i(w_dresbg, ddata,
						    dx);
						data += stride;
					}
				}
			}
	}
}

/* *********************************************************** */

#define	MLIB_LOAD_ALPHA32	MLIB_LOAD_ALPHA8

/* *********************************************************** */

#define	MLIB_MAKE_ALPHA32                                        \
	v_bg = *(mlib_f32 *)(data + x);                          \
	v_ubg = *(mlib_f32 *)(udata + ux);                       \
	v_dbg = *(mlib_f32 *)(ddata + dx);                       \
	w_resbg = vis_fmul8x16al(v_bg, vis_read_lo(w_div));      \
	w_uresbg = vis_fmul8x16al(v_ubg, vis_read_lo(w_udiv));   \
	w_dresbg = vis_fmul8x16al(v_dbg, vis_read_lo(w_ddiv));   \
	w_resbg = vis_fpadd16(w_ac, w_resbg);                    \
	w_uresbg = vis_fpadd16(w_ac, w_uresbg);                  \
	w_dresbg = vis_fpadd16(w_ac, w_dresbg);                  \
	w_resbg = vis_fpadd16(w_resc, w_resbg);                  \
	w_uresbg = vis_fpadd16(w_uresc, w_uresbg);               \
	w_dresbg = vis_fpadd16(w_dresc, w_dresbg);               \
	*(mlib_f32 *)(data + x) = vis_fpack16(w_resbg);          \
	*(mlib_f32 *)(udata + ux) = vis_fpack16(w_uresbg);       \
	*(mlib_f32 *)(ddata + dx) = vis_fpack16(w_dresbg)

/* *********************************************************** */

void
mlib_GraphicsSpamLineSet_A_32(
    mlib_u8 **rtable,
    mlib_s32 stride,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 c,
    mlib_d64 dc,
    mlib_s32 width,
    mlib_s32 height)
{
	mlib_u8 *atable, *table, *udata, *data, *ddata;
	mlib_s32 j, x, y, n, t, dt, offset, zz, ux, dx;
	mlib_d64 w_div = 0., w_udiv = 0., w_ddiv = 0.;
	mlib_f32 v_bg = 0.f, v_ubg = 0.f, v_dbg = 0.f;
	mlib_d64 w_c = dc, w_neg = vis_to_double_dup(255);
	mlib_d64 w_ac = vis_to_double(0xFF0000, 0);
	mlib_d64 w_01 = vis_to_double_dup(0x01);
	mlib_d64 w_resc, w_uresc, w_dresc, w_resbg, w_uresbg, w_dresbg;

	width *= 4;

	vis_write_gsr(7 << 3);

	for (; coords < coordsend; coords += MLIB_GDS_NEXT) {
		n = coords[MLIB_GDS_LENGTHLINES];
		y = coords[MLIB_GDS_STARTY];
		x = coords[MLIB_GDS_STARTX];
		dt = coords[MLIB_GDS_STEPLINES];
		zz = dt >> 31;
		zz = (dt ^ zz) - zz;
		atable =
		    mlib_GraphicsAliasTab_u8 +
		    512 * mlib_GraphicsSqrtTab_u8[zz >> (MLIB_SHIFT - 7)];
		table = atable + 256;

		if (n >= 0)
			if (y < 0) {
				t = ~y;

				if (dt) {
					x *= 4;

#pragma pipeloop(0)
					for (j = 0; j <= n; j++, x += 4) {
						offset =
						    (t >> (MLIB_SHIFT -
						    8)) & 0xFF;
						MLIB_LOAD_ALPHA32;
						y = t >> MLIB_SHIFT;
						t += dt;
						data = rtable[y];
						udata = rtable[y - 1];
						ddata = rtable[y + 1];
						ux = (((y - 1) >> 31) & ~3) | x;
						dx = (((height -
						    y) >> 31) & ~3) | x;
						MLIB_MAKE_ALPHA32;
					}
				} else {
					data = rtable[y = (t >> MLIB_SHIFT)];
					w_div =
					    vis_to_double_dup(2 *
					    MLIB_BG_COEFF);
					w_resc =
					    vis_fmul8x16al(vis_read_lo(w_c),
					    vis_read_lo(w_div));
					w_div = vis_fpsub16(w_neg, w_div);
					w_resc = vis_fpadd16(w_ac, w_resc);

					if (y > 0)
#pragma pipeloop(0)
						for (j = 0; j <= n; j++)
							MLIB_MAKE_HOR32(data,
							    4 * (x + j) -
							    stride);

					if (y <= height)
#pragma pipeloop(0)
						for (j = 0; j <= n; j++)
							MLIB_MAKE_HOR32(data,
							    4 * (x + j) +
							    stride);
					for (j = 0; j <= n; j++)
						*(mlib_s32 *)(data + 4 * (x +
						    j)) = c;
				}
			} else {
				t = ~x;

				if (dt & (MLIB_ONE - 1)) {
					y *= 4;

#pragma pipeloop(0)
					for (j = 0; j <= n; j++, y += 4) {
						offset =
						    (t >> (MLIB_SHIFT -
						    8)) & 0xFF;
						MLIB_LOAD_ALPHA32;
						x = (t >> (MLIB_SHIFT -
						    2)) & ~3;
						t += dt;
						ux = x - 4;
						dx = ((width - x) >> 31) & ~3;
						data =
						    *(mlib_u8 **)((mlib_u8 *)
						    rtable +
						    MLIB_POINTER_MUL(y));
						udata =
						    *(mlib_u8 **)((mlib_u8 *)
						    rtable +
						    MLIB_POINTER_MUL(y | ((ux >>
						    31) & ~3)));
						ddata =
						    *(mlib_u8 **)((mlib_u8 *)
						    rtable +
						    MLIB_POINTER_MUL(y | dx));
						dx = (x + 4) | dx;
						MLIB_MAKE_ALPHA32;
					}
				} else {
					y *= 4;
					data =
					    *(mlib_u8 **)((mlib_u8 *)rtable +
					    MLIB_POINTER_MUL(y));
					x = (t >> (MLIB_SHIFT - 2)) & ~3;
					dt = (dt >> (MLIB_SHIFT - 2)) & ~3;
					w_div = vis_ld_u8_i(atable, 128);
					w_div = vis_fpadd16(w_div, w_div);
					w_resc =
					    vis_fmul8x16al(vis_read_lo(w_c),
					    vis_read_lo(w_div));
					w_div = vis_fpsub16(w_neg, w_div);
					w_resc = vis_fpadd16(w_ac, w_resc);

					if (!dt) {
						data += x;

						if (x > 0)
#pragma pipeloop(0)
							for (j = 0; j <= n; j++)
								MLIB_MAKE_HOR32
								    (data,
								    j * stride -
								    4)

							if (x <= width)
#pragma pipeloop(0)
							    for (j = 0; j <= n;
								j++)
								MLIB_MAKE_HOR32
								    (data, j *
								    stride + 4)
#pragma pipeloop(0)
							    for (j = 0; j <= n;
								j++)
								*(mlib_s32 *)
								(data + j *
								stride) = c;
					} else {
						if (x > 0)
							MLIB_MAKE_HOR32(data,
							    x - 4);

						if (x <= width)
							MLIB_MAKE_HOR32(data,
							    x + 4);
						*(mlib_s32 *)(data + x) = c;

						if (!n)
							continue;
						n--;

						x += dt;
						data += stride;

#pragma pipeloop(0)
						for (j = 0; j < n; j++)
							*(mlib_s32 *)(data + x +
							    j * (stride + dt)) =
							    c;

#pragma pipeloop(0)
						for (j = 0; j < n; j++)
							MLIB_MAKE_HOR32(data,
							    x + j * (stride +
							    dt) - 4);

#pragma pipeloop(0)
						for (j = 0; j < n; j++)
							MLIB_MAKE_HOR32(data,
							    x + j * (stride +
							    dt) + 4);

						if (dt > 0)
							x += (n << 2);
						else
							x -= (n << 2);

						*(mlib_s32 *)(data + x +
						    j * (stride)) = c;

						if (x > 0)
							MLIB_MAKE_HOR32(data,
							    x + j * (stride) -
							    4);

						if (x <= width)
							MLIB_MAKE_HOR32(data,
							    x + j * (stride) +
							    4);
					}
				}
			}
	}
}

/* *********************************************************** */
