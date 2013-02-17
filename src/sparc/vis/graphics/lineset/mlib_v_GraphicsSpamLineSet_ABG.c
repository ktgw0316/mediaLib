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

#pragma ident	"@(#)mlib_v_GraphicsSpamLineSet_ABG.c	9.2	07/11/05 SMI"

/*
 *  FUNCTIONS
 *    mlib_GraphicsSpamLineSet_ABG_8  - line drawing function for mlib_u8 image
 *    mlib_GraphicsSpamLineSet_ABG_32 - line drawing function for mlib_s32 image
 *
 *  SYNOPSIS
 *    void  mlib_GraphicsSpamLineSet_ABG_8
 *                                    (mlib_u8  **rtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *				       mlib_s32 alpha,
 *                                     mlib_s32 width,
 *                                     mlib_s32 height);
 *    void  mlib_GraphicsSpamLineSet_ABG_32
 *                                    (mlib_u8  **rtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *				       mlib_s32 alpha,
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
 *    alpha	  Alpha blending value.
 */

#include <mlib_graphics.h>
#include <mlib_GraphicsDrawSet.h>
#include <vis_proto.h>
#include <stdio.h>

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

#define	MLIB_LOAD_ALPHA8                                                  \
	w_tc = vis_faligndata(w_c, w_c);                                  \
	w_c = vis_fpadd16(w_c, w_dc);                                     \
	w_div = vis_ld_u8_i(table, offset);                               \
	w_udiv = vis_ld_u8_i(atable, offset ^ 0xFF);                      \
	w_ddiv = vis_ld_u8_i(atable, offset);                             \
	w_div = vis_fpadd16(w_div, w_div);                                \
	w_div = vis_fpsub16(w_div, w_01);                                 \
	w_udiv = vis_fpadd16(w_udiv, w_udiv);                             \
	w_ddiv = vis_fpadd16(w_ddiv, w_ddiv);                             \
	w_resc = vis_fmul8x16al(vis_read_lo(w_tc), vis_read_lo(w_div));   \
	w_uresc =                                                         \
	vis_fmul8x16al(vis_read_lo(w_tc), vis_read_lo(w_udiv));           \
	w_dresc =                                                         \
	vis_fmul8x16al(vis_read_lo(w_tc), vis_read_lo(w_ddiv));           \
	w_div = vis_fpsub16(w_neg, w_div);                                \
	w_udiv = vis_fpsub16(w_neg, w_udiv);                              \
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
	w_resfg = vis_fpadd16(w_resc, w_resbg);                            \
	w_uresfg = vis_fpadd16(w_uresc, w_uresbg);                         \
	w_dresfg = vis_fpadd16(w_dresc, w_dresbg);                         \
	w_resbg =                                                          \
	vis_fpadd16(vis_fmul8x16(vis_read_lo(w_bg), donema),               \
	    vis_fmul8x16(fa, w_resfg));                                    \
	vis_st_u8_i(w_resbg, data, x);                                     \
	w_uresbg =                                                         \
	vis_fpadd16(vis_fmul8x16(vis_read_lo(w_ubg), donema),              \
	    vis_fmul8x16(fa, w_uresfg));                                   \
	vis_st_u8_i(w_uresbg, udata, ux);                                  \
	w_dresbg =                                                         \
	vis_fpadd16(vis_fmul8x16(vis_read_lo(w_dbg), donema),              \
	    vis_fmul8x16(fa, w_dresfg));                                   \
	vis_st_u8_i(w_dresbg, ddata, dx);                                  \
	w_div = vis_fand(w_ff, w_div);

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

#define	MLIB_CYCLE_HOR8(UX, DX)                                            \
	{                                                                  \
	    w_tc = vis_faligndata(w_c, w_c);                               \
	    w_c = vis_fpadd16(w_c, w_dc);                                  \
	    w_resc =                                                       \
		vis_fmul8x16al(vis_read_lo(w_tc), vis_read_lo(w_udiv));    \
	    w_bg = vis_ld_u8_i(data, 0);                                   \
	    w_ubg = vis_ld_u8_i(data, UX);                                 \
	    w_dbg = vis_ld_u8_i(data, DX);                                 \
	    w_uresbg =                                                     \
		vis_fmul8x16al(vis_read_lo(w_ubg), vis_read_lo(w_ddiv));   \
	    w_dresbg =                                                     \
		vis_fmul8x16al(vis_read_lo(w_dbg), vis_read_lo(w_ddiv));   \
	    w_uresfg = vis_fpadd16(w_resc, w_uresbg);                      \
	    w_dresfg = vis_fpadd16(w_resc, w_dresbg);                      \
	    w_uresbg =                                                     \
		vis_fpadd16(vis_fmul8x16(vis_read_lo(w_ubg), donema),      \
		vis_fmul8x16(fa, w_uresfg));                               \
	    vis_st_u8_i(w_uresbg, data, UX);                               \
	    w_dresbg =                                                     \
		vis_fpadd16(vis_fmul8x16(vis_read_lo(w_dbg), donema),      \
		vis_fmul8x16(fa, w_dresfg));                               \
	    vis_st_u8_i(w_dresbg, data, DX);                               \
	    w_resbg =                                                      \
		vis_fpadd16(vis_fmul8x16(vis_read_lo(w_bg), donema),       \
		vis_fmul8x16al(vis_read_lo(w_tc), fa1));                   \
	    vis_st_u8_i(w_resbg, data, 0);                                 \
	}

/* *********************************************************** */

void
mlib_GraphicsSpamLineSet_ABG_8(
    mlib_u8 **rtable,
    mlib_s32 stride,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 a,
    mlib_s32 width,
    mlib_s32 height)
{
	mlib_u8 *atable, *table, *udata, *data, *ddata;
	mlib_s32 j, x, y, n, t, dt, offset, zz, ux, dx;
	mlib_d64 w_div = 0., w_udiv = 0., w_ddiv = 0.;
	mlib_d64 w_bg = 0., w_ubg = 0., w_dbg = 0.;
	mlib_d64 w_c = 0., w_tc, w_dc = 0., w_neg = vis_to_double_dup(255);
	mlib_d64 w_01 = vis_to_double_dup(0x01);
	mlib_d64 w_ff = vis_to_double_dup(0xFF00FF);
	mlib_d64 w_80 = vis_to_double_dup(0x80008000);
	mlib_d64 w_02 = vis_to_double_dup(0x20002);
	mlib_d64 w_resc, w_uresc, w_dresc, w_resbg, w_uresbg, w_dresbg, w_resfg,
	    w_uresfg, w_dresfg;

	mlib_s32 sa, sonema;
	mlib_f32 fa, fa1, fonema, ld, rgbmask = vis_to_float(0xFFFFFF);
	mlib_d64 donema, donema1, m0, m1, sum;
	mlib_f32 falpha = vis_to_float(0xFF000000), st;

	sa = a & 0xFF;
	sonema = ~a & 0xFF;
	sa = (sa << 24) | (sa << 16) | (sa << 8) | sa;
	fa = vis_to_float(sa);
	fa1 = vis_read_lo(vis_fpmerge(vis_fzeros(), vis_to_float(sa)));

	donema = vis_to_double(sonema, sonema);
	donema1 =
	    vis_to_double((sonema << 16) | sonema, (sonema << 16) | sonema);

	vis_write_gsr((7 << 3) | 7);

#pragma unroll(1)
	for (; coords < coordsend; coords += MLIB_GDS_NEXT_GZ) {
		n = coords[MLIB_GDS_LENGTHLINES];
		y = coords[MLIB_GDS_STARTY];
		x = coords[MLIB_GDS_STARTX];
		dt = coords[MLIB_GDS_STEPLINES];
		w_c =
		    vis_freg_pair(vis_read_hi(w_c),
		    ((mlib_f32 *)coords)[MLIB_GDS_STARTC]);
		w_dc =
		    vis_freg_pair(vis_read_hi(w_dc),
		    ((mlib_f32 *)coords)[MLIB_GDS_STEPC]);
		w_c = vis_fnot(w_c);
		w_dc = vis_fpadd32(w_dc, w_dc);
		w_c = vis_fpadd32(w_c, w_c);
		w_dc = vis_faligndata(w_dc, w_dc);
		w_c = vis_faligndata(w_c, w_c);
		w_dc =
		    vis_fpsub16(w_dc, vis_fmul8ulx16(w_02, vis_fand(w_80,
		    w_dc)));
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
					w_udiv =
					    vis_to_double_dup(2 *
					    MLIB_BG_COEFF);
					w_ddiv = vis_fpsub16(w_neg, w_udiv);
					n++;
					data += x;
					ddata = data + n;

					if ((y <= 0) || (y > height) ||
					    (stride & 3)) {
						ux = (y > 0) ? -stride : 0;
						dx = (y <=
						    height) ? +stride : 0;

#pragma pipeloop(0)
						for (; data < ddata; data++)
							MLIB_CYCLE_HOR8(ux, dx);

					} else {
						w_div =
						    vis_to_double_dup(2 *
						    MLIB_BG_COEFF |
						    (MLIB_BG_COEFF << 17) |
						    (MLIB_BG_COEFF << 9) |
						    (MLIB_BG_COEFF << 25));

						udata =
						    (mlib_u8 *)(((mlib_addr)data
						    + 3) & ~3);
						ddata = data + n;

						if (udata + 4 < ddata) {
							mlib_f32 v_coeff1 =
							    vis_to_float
							    (0xffffffff),
							    v_coeff2 =
							    vis_to_float
							    (0x004080C0);
							mlib_d64 w_c4, w_dc4;

							for (; data < udata;
							    data++)
								MLIB_CYCLE_HOR8
								    (-stride,
								    +stride);

							udata =
							    (mlib_u8
							    *)((mlib_addr)ddata
							    & ~3);

							w_c4 =
							    vis_fmul8x16al
							    (v_coeff1,
							    vis_read_lo(w_c));
							w_dc4 =
							    vis_fmul8x16al
							    (v_coeff2,
							    vis_read_lo(w_dc));
							w_dc4 =
							    vis_fpadd16(w_dc4,
							    w_dc4);
							w_dc4 =
							    vis_fpadd16(w_dc4,
							    w_dc4);
							w_c4 =
							    vis_fpadd16(w_c4,
							    w_dc4);
							w_dc4 =
							    vis_fmul8x16al
							    (v_coeff1,
							    vis_read_lo(w_dc));
							w_dc4 =
							    vis_fpadd16(w_dc4,
							    w_dc4);
							w_dc4 =
							    vis_fpadd16(w_dc4,
							    w_dc4);

#pragma pipeloop(0)
							for (; data < udata;
							    data += 4) {
								mlib_f32 v_bg,
								    v_ubg,
								    v_dbg;

							w_tc = vis_faligndata
								    (w_c4,
								    w_c4);
								w_c4 =
								    vis_fpadd16
								    (w_c4,
								    w_dc4);
								w_tc =
								    vis_fand
								    (w_tc,
								    w_ff);
								w_resc =
								    vis_fmul8x16
								    (vis_read_lo
								    (w_div),
								    w_tc);
								v_bg =
								    *(mlib_f32
								    *)(data);
								v_ubg =
								    *(mlib_f32
								    *)(data -
								    stride);
								v_dbg =
								    *(mlib_f32
								    *)(data +
								    stride);
						w_uresbg = vis_fmul8x16al
								    (v_ubg,
								    vis_read_lo
								    (w_ddiv));
						w_dresbg = vis_fmul8x16al
								    (v_dbg,
								    vis_read_lo
								    (w_ddiv));
								w_uresfg =
								    vis_fpadd16
								    (w_resc,
								    w_uresbg);
								w_dresfg =
								    vis_fpadd16
								    (w_resc,
								    w_dresbg);

							w_resbg = vis_fpadd16
							    (vis_fmul8x16
							    (v_bg, donema1),
								    vis_fmul8x16
								    (fa, w_tc));
							w_uresbg = vis_fpadd16
							    (vis_fmul8x16
							    (v_ubg, donema1),
								    vis_fmul8x16
								    (fa,
								    w_uresfg));
							w_dresbg = vis_fpadd16
							    (vis_fmul8x16
							    (v_dbg, donema1),
								    vis_fmul8x16
								    (fa,
								    w_dresfg));

								*(mlib_f32
								    *)(data) =
								    vis_fpack16
								    (w_resbg);
								*(mlib_f32
								    *)(data -
								    stride) =
								    vis_fpack16
								    (w_uresbg);
								*(mlib_f32
								    *)(data +
								    stride) =
								    vis_fpack16
								    (w_dresbg);
							}

							if (data < ddata) {
								mlib_f32 v_bg,
								    v_ubg,
								    v_dbg;

					w_tc = vis_faligndata(w_c4, w_c4);
								w_c4 =
								    vis_fpadd16
								    (w_c4,
								    w_dc4);
								w_tc =
								    vis_fand
								    (w_tc,
								    w_ff);
								w_resc =
								    vis_fmul8x16
								    (vis_read_lo
								    (w_div),
								    w_tc);
								v_bg =
								    *(mlib_f32
								    *)(data);
								v_ubg =
								    *(mlib_f32
								    *)(data -
								    stride);
								v_dbg =
								    *(mlib_f32
								    *)(data +
								    stride);
								w_uresbg =
							    vis_fmul8x16al
								    (v_ubg,
								    vis_read_lo
								    (w_ddiv));
								w_dresbg =
							    vis_fmul8x16al
								    (v_dbg,
								    vis_read_lo
								    (w_ddiv));
								w_uresfg =
								    vis_fpadd16
								    (w_resc,
								    w_uresbg);
								w_dresfg =
								    vis_fpadd16
								    (w_resc,
								    w_dresbg);

								w_resbg =
								    vis_fpadd16
							    (vis_fmul8x16
								    (v_bg,
								    donema1),
								    vis_fmul8x16
								    (fa, w_tc));
								w_uresbg =
								    vis_fpadd16
							    (vis_fmul8x16
								    (v_ubg,
								    donema1),
								    vis_fmul8x16
								    (fa,
								    w_uresfg));
								w_dresbg =
								    vis_fpadd16
							    (vis_fmul8x16
								    (v_dbg,
								    donema1),
								    vis_fmul8x16
								    (fa,
								    w_dresfg));

								v_bg =
								    vis_fpack16
								    (w_resbg);
								v_ubg =
								    vis_fpack16
								    (w_uresbg);
								v_dbg =
								    vis_fpack16
								    (w_dresbg);

								ddata--;
								vis_pst_8
							    (vis_freg_pair
								    (v_bg,
								    v_bg),
								    (void
								    *)(
								    (mlib_addr)
								    data & ~7),
								    vis_edge8
								    (data,
								    ddata));
								data -= stride;
								ddata -= stride;
								vis_pst_8
							    (vis_freg_pair
								    (v_ubg,
								    v_ubg),
								    (void
								    *)(
								    (mlib_addr)
								    data & ~7),
								    vis_edge8
								    (data,
								    ddata));
								data +=
								    2 * stride;
								ddata +=
								    2 * stride;
								vis_pst_8
							    (vis_freg_pair
								    (v_dbg,
								    v_dbg),
								    (void
								    *)(
								    (mlib_addr)
								    data & ~7),
								    vis_edge8
								    (data,
								    ddata));
							}
						} else {
							for (; data < ddata;
							    data++)
								MLIB_CYCLE_HOR8
								    (-stride,
								    +stride);
						}
					}
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
					w_ddiv = vis_fpsub16(w_neg, w_udiv);

#pragma pipeloop(0)
					for (j = 0; j <= n; j++, y++, x += dt) {
						w_tc = vis_faligndata(w_c, w_c);
						w_c = vis_fpadd16(w_c, w_dc);
						w_uresc =
						    vis_fmul8x16al(vis_read_lo
						    (w_tc),
						    vis_read_lo(w_udiv));
						ux = x - 1;
						dx = (width - x) >> 31;
						udata = rtable[y | (ux >> 31)];
						ddata = rtable[y | dx];
						dx = (x + 1) | dx;
						w_bg = vis_ld_u8_i(data, x);
						w_ubg = vis_ld_u8_i(udata, ux);
						w_dbg = vis_ld_u8_i(ddata, dx);
						w_uresbg =
						    vis_fmul8x16al(vis_read_lo
						    (w_ubg),
						    vis_read_lo(w_ddiv));
						w_dresbg =
						    vis_fmul8x16al(vis_read_lo
						    (w_dbg),
						    vis_read_lo(w_ddiv));
						w_uresfg =
						    vis_fpadd16(w_uresc,
						    w_uresbg);
						w_dresfg =
						    vis_fpadd16(w_uresc,
						    w_dresbg);

						w_resbg =
						    vis_fpadd16(vis_fmul8x16
						    (vis_read_lo(w_bg), donema),
						    vis_fmul8x16al(vis_read_lo
						    (w_tc), fa1));
						vis_st_u8_i(w_resbg, data, x);
						w_uresbg =
						    vis_fpadd16(vis_fmul8x16
						    (vis_read_lo(w_ubg),
						    donema), vis_fmul8x16(fa,
						    w_uresfg));
						vis_st_u8_i(w_uresbg, udata,
						    ux);
						w_dresbg =
						    vis_fpadd16(vis_fmul8x16
						    (vis_read_lo(w_dbg),
						    donema), vis_fmul8x16(fa,
						    w_dresfg));
						vis_st_u8_i(w_dresbg, ddata,
						    dx);
						data += stride;
					}
				}
			}
	}
}

/* *********************************************************** */

#define	MLIB_LOAD_ALPHA32                                       \
	w_tc = vis_faligndata(w_c, w_c);                        \
	w_c = vis_fpadd16(w_c, w_dc);                           \
	w_tc = vis_fand(w_tc, w_ff);                            \
	v_tc = vis_fpack16(w_tc);                               \
	w_div = vis_ld_u8_i(table, offset);                     \
	w_udiv = vis_ld_u8_i(atable, offset ^ 0xFF);            \
	w_ddiv = vis_ld_u8_i(atable, offset);                   \
	w_div = vis_fpadd16(w_div, w_div);                      \
	w_div = vis_fpsub16(w_div, w_01);                       \
	w_udiv = vis_fpadd16(w_udiv, w_udiv);                   \
	w_ddiv = vis_fpadd16(w_ddiv, w_ddiv);                   \
	w_resc = vis_fmul8x16al(v_tc, vis_read_lo(w_div));      \
	w_uresc = vis_fmul8x16al(v_tc, vis_read_lo(w_udiv));    \
	w_dresc = vis_fmul8x16al(v_tc, vis_read_lo(w_ddiv));    \
	w_div = vis_fpsub16(w_neg, w_div);                      \
	w_udiv = vis_fpsub16(w_neg, w_udiv);                    \
	w_ddiv = vis_fpsub16(w_neg, w_ddiv)

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
	m0 = vis_fmul8x16(v_bg, donema);                         \
	m1 = vis_fmul8x16(fa, w_resbg);                          \
	w_resbg = vis_fpadd16(m0, m1);                           \
	m0 = vis_fmul8x16(v_ubg, donema);                        \
	m1 = vis_fmul8x16(fa, w_uresbg);                         \
	w_uresbg = vis_fpadd16(m0, m1);                          \
	m0 = vis_fmul8x16(v_dbg, donema);                        \
	m1 = vis_fmul8x16(fa, w_dresbg);                         \
	w_dresbg = vis_fpadd16(m0, m1);                          \
	*(mlib_f32 *)(data + x) =                                \
	vis_fors(falpha, vis_fpack16(w_resbg));                  \
	*(mlib_f32 *)(udata + ux) =                              \
	vis_fors(falpha, vis_fpack16(w_uresbg));                 \
	*(mlib_f32 *)(ddata + dx) =                              \
	vis_fors(falpha, vis_fpack16(w_dresbg));

/* *********************************************************** */

void
mlib_GraphicsSpamLineSet_ABG_32(
    mlib_u8 **rtable,
    mlib_s32 stride,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 a,
    mlib_s32 width,
    mlib_s32 height)
{
	mlib_u8 *atable, *table, *udata, *data, *ddata;
	mlib_s32 j, x, y, n, t, dt, offset, zz, ux, dx;
	mlib_d64 w_div = 0., w_udiv = 0., w_ddiv = 0.;
	mlib_f32 v_bg = 0.f, v_ubg = 0.f, v_dbg = 0.f, v_fg, v_ufg, v_dfg;
	mlib_d64 w_c, w_neg = vis_to_double_dup(255);
	mlib_d64 w_c2, w_dc2, w_dc, w_tc;
	mlib_d64 w_const = vis_to_double(0x7FFFFFFF, 0xFFFFFFFF);
	mlib_d64 w_01 = vis_to_double_dup(0x01);
	mlib_d64 w_ff = vis_to_double_dup(0xFF00FF);
	mlib_d64 w_ac = vis_to_double(0xFF0000, 0);
	mlib_d64 w_80 = vis_to_double_dup(0x80008000);
	mlib_d64 w_02 = vis_to_double_dup(0x20002);
	mlib_d64 w_resc, w_uresc, w_dresc, w_resbg, w_uresbg, w_dresbg;
	mlib_s32 sa, sonema;
	mlib_f32 fa, fonema, ld, rgbmask = vis_to_float(0xFFFFFF);
	mlib_d64 donema, m0, m1, sum;
	mlib_f32 falpha = vis_to_float(0xFF000000), st;

	sa = a & 0xFF;
	sonema = ~a & 0xFF;
	sa = (sa << 16) | (sa << 8) | sa;
	fa = vis_to_float(sa);

	donema = vis_to_double(sonema, (sonema << 16) | sonema);

	width *= 4;

	vis_write_gsr((7 << 3) | 7);

	for (; coords < coordsend; coords += MLIB_GDS_NEXT_GZ32) {
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
		w_c2 = vis_to_double(0, ((mlib_s32 *)coords)[MLIB_GDS_STARTC2]);
		w_c = vis_freg_pair(((mlib_f32 *)coords)[MLIB_GDS_STARTC1],
		    ((mlib_f32 *)coords)[MLIB_GDS_STARTC]);
		w_dc2 = vis_to_double(0, ((mlib_s32 *)coords)[MLIB_GDS_STEPC2]);
		w_dc = vis_freg_pair(((mlib_f32 *)coords)[MLIB_GDS_STEPC1],
		    ((mlib_f32 *)coords)[MLIB_GDS_STEPC]);
		w_c2 = vis_fxor(w_c2, w_const);
		w_c = vis_fnot(w_c);
		w_c = vis_fpadd32(w_c, w_c);
		w_dc = vis_fpadd32(w_dc, w_dc);
		w_c2 = vis_fpadd32(w_c2, w_c2);
		w_dc2 = vis_fpadd32(w_dc2, w_dc2);

		w_dc = vis_fpackfix_pair(w_dc2, w_dc);
		w_c = vis_fpackfix_pair(w_c2, w_c);

		w_dc =
		    vis_fpsub16(w_dc, vis_fmul8ulx16(w_02, vis_fand(w_80,
		    w_dc)));
		w_c = vis_fpadd16(w_c, w_c);
		w_dc = vis_fpadd16(w_dc, w_dc);

		if (n >= 0)
			if (y < 0) {
				t = ~y;

				if (dt) {
					x *= 4;

#pragma pipeloop(0)
					for (j = 0; j <= n; j++, x += 4) {
						mlib_f32 v_tc;

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
					data = rtable[y = t >> MLIB_SHIFT];
					ux = (y > 0) ? -stride : 0;
					dx = (y <= height) ? +stride : 0;
					data += 4 * x;
					udata = data + 4 * n;
					w_udiv = vis_to_double_dup
						(2 * MLIB_BG_COEFF);
					w_ddiv = vis_fpsub16(w_neg, w_udiv);
					w_div = vis_to_double_dup
					(2 * MLIB_BG_COEFF |
					(MLIB_BG_COEFF << 17) |
					(MLIB_BG_COEFF << 9));

#pragma pipeloop(0)
					for (; data <= udata; data += 4) {
					w_tc = vis_faligndata(w_c, w_c);
					w_c = vis_fpadd16(w_c, w_dc);
					w_tc = vis_fand(w_tc, w_ff);
					w_resc = vis_fmul8x16
						(vis_read_lo(w_div), w_tc);
					w_resc = vis_fpadd16(w_resc, w_ac);
					v_bg = *(mlib_f32 *)(data);
					v_ubg = *(mlib_f32 *)(data + ux);
					v_dbg = *(mlib_f32 *)(data + dx);
					w_uresbg = vis_fmul8x16al
						(v_ubg, vis_read_lo(w_ddiv));
					w_dresbg = vis_fmul8x16al
						(v_dbg, vis_read_lo(w_ddiv));
					m0 = vis_fmul8x16(v_bg, donema);
					m1 = vis_fmul8x16(fa, w_tc);
					w_resbg = vis_fpadd16(m0, m1);
					w_uresbg = vis_fpadd16
						(w_resc, w_uresbg);
					m0 = vis_fmul8x16(v_ubg, donema);
					m1 = vis_fmul8x16(fa, w_uresbg);
					w_uresbg = vis_fpadd16(m0, m1);
					w_dresbg = vis_fpadd16
						(w_resc, w_dresbg);
					m0 = vis_fmul8x16(v_dbg, donema);
					m1 = vis_fmul8x16(fa, w_dresbg);
					w_dresbg = vis_fpadd16(m0, m1);
					*(mlib_f32 *)(data + ux) = vis_fors
						(falpha, vis_fpack16(w_uresbg));
					*(mlib_f32 *)(data + dx) = vis_fors
						(falpha, vis_fpack16(w_dresbg));
					*(mlib_f32 *)(data) = vis_fors
						(falpha, vis_fpack16(w_resbg));
					}
				}
			} else {
				t = ~x;

				if (dt & (MLIB_ONE - 1)) {
					y *= 4;

#pragma pipeloop(0)
					for (j = 0; j <= n; j++, y += 4) {
						mlib_f32 v_tc;

						offset =
						    (t >> (MLIB_SHIFT -
						    8)) & 0xFF;
						MLIB_LOAD_ALPHA32;
						x = (t >> (MLIB_SHIFT -
						    2)) & ~3;
						t += dt;
						ux = x - 4;
						dx = ((width - x) >> 31) & ~3;
						data = *(mlib_u8 **)((mlib_u8 *)
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
					x = (t >> (MLIB_SHIFT - 2)) & ~3;
					data =
					    *(mlib_u8 **)((mlib_u8 *)rtable +
					    MLIB_POINTER_MUL(y));
					dt = (dt >> (MLIB_SHIFT - 2)) & ~3;
					w_udiv = vis_ld_u8_i(atable, 128);
					w_udiv = vis_fpadd16(w_udiv, w_udiv);
					w_ddiv = vis_fpsub16(w_neg, w_udiv);

					for (j = 0; j <= n; j++, x += dt) {
						mlib_f32 v_tc;

						w_tc = vis_faligndata(w_c, w_c);
						w_c = vis_fpadd16(w_c, w_dc);
						w_tc = vis_fand(w_tc, w_ff);
						v_tc = vis_fpack16(w_tc);
						w_uresc =
						    vis_fmul8x16al(v_tc,
						    vis_read_lo(w_udiv));
						w_uresc =
						    vis_fpadd16(w_uresc, w_ac);
						ux = x - 4;
						dx = ((width - x) >> 31) & ~3;
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
						y += 4;
						v_bg = *(mlib_f32 *)(data + x);
						v_ubg =
						    *(mlib_f32 *)(udata + ux);
						v_dbg =
						    *(mlib_f32 *)(ddata + dx);
						w_uresbg =
						    vis_fmul8x16al(v_ubg,
						    vis_read_lo(w_ddiv));
						w_dresbg =
						    vis_fmul8x16al(v_dbg,
						    vis_read_lo(w_ddiv));
						m0 = vis_fmul8x16(v_bg, donema);
						m1 = vis_fmul8x16(fa, w_tc);
						w_resbg = vis_fpadd16(m0, m1);
						w_uresbg =
						    vis_fpadd16(w_uresc,
						    w_uresbg);
						m0 = vis_fmul8x16(v_ubg,
						    donema);
						m1 = vis_fmul8x16(fa, w_uresbg);
						w_uresbg = vis_fpadd16(m0, m1);
						w_dresbg =
						    vis_fpadd16(w_uresc,
						    w_dresbg);
						m0 = vis_fmul8x16(v_dbg,
						    donema);
						m1 = vis_fmul8x16(fa, w_dresbg);
						w_dresbg = vis_fpadd16(m0, m1);
						*(mlib_f32 *)(data + x) =
						    vis_fors(falpha,
						    vis_fpack16(w_resbg));
						*(mlib_f32 *)(udata + ux) =
						    vis_fors(falpha,
						    vis_fpack16(w_uresbg));
						*(mlib_f32 *)(ddata + dx) =
						    vis_fors(falpha,
						    vis_fpack16(w_dresbg));
						data += stride;
					}
				}
			}
	}
}

/* *********************************************************** */
