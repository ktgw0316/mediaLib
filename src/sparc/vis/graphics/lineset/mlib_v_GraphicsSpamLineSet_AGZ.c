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

#pragma ident	"@(#)mlib_v_GraphicsSpamLineSet_AGZ.c	9.2	07/11/05 SMI"

/*
 *  FUNCTIONS
 *    mlib_GraphicsSpamLineSet_AGZ_8  - line drawing function for mlib_u8 image
 *    mlib_GraphicsSpamLineSet_AGZ_32 - line drawing function for mlib_s32 image
 *
 *  SYNOPSIS
 *    void  mlib_GraphicsSpamLineSet_AGZ_8
 *                                    (mlib_u8  **rtable,
 *                                     mlib_u8  **zrtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 zstride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 width,
 *                                     mlib_s32 height);
 *    void  mlib_GraphicsSpamLineSet_AGZ_32
 *                                    (mlib_u8  **rtable,
 *                                     mlib_u8  **zrtable,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 zstride,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 width,
 *                                     mlib_s32 height);
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
 *    width       width = Image.Width - 2, is using to discover
 *                to clip neighbour in *Spam_A* functions
 *    height      height = Image.Height - 2, is using to discover
 *                to clip neighbour in *Spam_A* functions
 */

#include <mlib_graphics.h>
#include <mlib_GraphicsDrawSet.h>
#include <vis_proto.h>

/* *********************************************************** */

#ifdef _LP64

#define	MLIB_GET_POINTER(RT, T)                                            \
	* (mlib_u8 **)((mlib_u8 *)RT + (((T) >> (MLIB_SHIFT - 3)) & ~7))
#define	MLIB_GET_POINTER2(RT, T, ZZ)	*(mlib_u8**)((mlib_u8*)RT +   \
					((((T) >> (MLIB_SHIFT - 3)) | \
						ZZ) &~ 7))
#define	MLIB_POINTER_COEF	8
#define	MLIB_POINTER_MUL(X)	2 * (X)

#else /* _LP64 */

#define	MLIB_GET_POINTER(RT, T)                                            \
	* (mlib_u8 **)((mlib_u8 *)RT + (((T) >> (MLIB_SHIFT - 2)) & ~3))
#define	MLIB_GET_POINTER2(RT, T, ZZ)	*(mlib_u8**)((mlib_u8*)RT +   \
					((((T) >> (MLIB_SHIFT - 2)) | \
						ZZ) &~ 3))
#define	MLIB_POINTER_COEF	4
#define	MLIB_POINTER_MUL(X)	(X)
#endif /* _LP64 */

/* *********************************************************** */

#define	MLIB_LOAD_ALPHA8                                                  \
	w_tc = vis_faligndata(w_c, w_c);                                  \
	w_c = vis_fpadd16(w_c, w_dc);                                     \
	w_div = vis_ld_u8_i(atable, offset + 256);                        \
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
	w_resbg = vis_fpadd16(w_resc, w_resbg);                            \
	w_uresbg = vis_fpadd16(w_uresc, w_uresbg);                         \
	w_dresbg = vis_fpadd16(w_dresc, w_dresbg);                         \
	zdata[xz] = stz;                                                   \
	vis_st_u8_i(w_resbg, data, xz);                                    \
	vis_st_u8_i(w_uresbg, udata, ux);                                  \
	vis_st_u8_i(w_dresbg, ddata, dx);                                  \
	w_div = vis_fand(w_ff, w_div)

/* *********************************************************** */

void
mlib_GraphicsSpamLineSet_AGZ_8(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 stride,
    mlib_s32 zstride,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 width,
    mlib_s32 height)
{
	mlib_u8 *atable, *udata, *data, *ddata;
	mlib_s16 *zdata, *zdata1;
	mlib_s32 j, x, y, z, n, t, dt, dz, stz, offset, zz, xz, ux, dx;
	mlib_s32 ualpha, dalpha;
	mlib_d64 w_div = 0., w_udiv = 0., w_ddiv = 0.;
	mlib_d64 w_bg = 0., w_ubg = 0., w_dbg = 0.;
	mlib_d64 w_c = 0., w_tc = 0., w_dc, w_neg = vis_to_double_dup(255);
	mlib_d64 w_01 = vis_to_double_dup(0x01);
	mlib_d64 w_ff = vis_to_double_dup(0xFF00FF);
	mlib_d64 w_80 = vis_to_double_dup(0x80008000);
	mlib_d64 w_02 = vis_to_double_dup(0x20002);
	mlib_d64 w_resc, w_uresc, w_dresc, w_resbg, w_uresbg, w_dresbg;

	vis_write_gsr((7 << 3) | 7);

	for (; coords < coordsend; coords += MLIB_GDS_NEXT_GZ) {
		n = coords[MLIB_GDS_LENGTHLINES];
		y = coords[MLIB_GDS_STARTY];
		x = coords[MLIB_GDS_STARTX];
		dt = coords[MLIB_GDS_STEPLINES];
		z = ~coords[MLIB_GDS_STARTZ];
		dz = coords[MLIB_GDS_STEPZ];
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

		if (n >= 0)
			if (y < 0) {
				t = ~y;
				zdata = (mlib_s16 *)(zrtable[y =
				    t >> MLIB_SHIFT]);

				if (dt) {
					offset = (t >> (MLIB_SHIFT - 8)) & 0xFF;

#pragma pipeloop(0)
					for (j = 0; j <= n; j++, x++) {
						stz = z >> MLIB_SHIFT;
						z += dz;
						zdata =
						    (mlib_s16
						    *)(MLIB_GET_POINTER(zrtable,
						    t));
						zz = (zdata[x] - stz) >> 31;
						MLIB_LOAD_ALPHA8;
						y = t >> MLIB_SHIFT;
						t += dt;
						xz = x | zz;
						data = rtable[y | zz];
						zdata =
						    (mlib_s16 *)(zrtable[y |
						    zz]);
						udata = rtable[(y - 1) | zz];
						ddata = rtable[(y + 1) | zz];
						ux = ((y - 1) >> 31) | xz;
						dx = ((height - y) >> 31) | xz;
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
					ualpha = (y - 1) >> 31;
					dalpha = (height - y) >> 31;

#pragma pipeloop(0)
					for (j = 0; j <= n; j++, x++) {
						w_tc = vis_faligndata(w_c, w_c);
						w_c = vis_fpadd16(w_c, w_dc);
						w_resc =
						    vis_fmul8x16al(vis_read_lo
						    (w_tc),
						    vis_read_lo(w_udiv));
						stz = z >> MLIB_SHIFT;
						z += dz;
						zz = (zdata[x] - stz) >> 31;
						ux = ualpha | zz;
						dx = dalpha | zz;
						data = rtable[y | zz];
						zdata1 =
						    (mlib_s16 *)(zrtable[y |
						    zz]);
						udata = rtable[(y - 1) | ux];
						ddata = rtable[(y + 1) | dx];
						xz = x | zz;
						ux = x | ux;
						dx = x | dx;
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
						zdata1[xz] = stz;
						w_uresbg =
						    vis_fpadd16(w_resc,
						    w_uresbg);
						w_dresbg =
						    vis_fpadd16(w_resc,
						    w_dresbg);
						vis_st_u8_i(w_tc, data, xz);
						vis_st_u8_i(w_uresbg, udata,
						    ux);
						vis_st_u8_i(w_dresbg, ddata,
						    dx);
					}
				}
			} else {
				t = ~x;
				x = t >> MLIB_SHIFT;

				if (dt & (MLIB_ONE - 1)) {
					offset = (t >> (MLIB_SHIFT - 8)) & 0xFF;

#pragma pipeloop(0)
					for (j = 0; j <= n; j++, y++) {
						stz = z >> MLIB_SHIFT;
						z += dz;
						x = t >> MLIB_SHIFT;
						zdata =
						    (mlib_s16 *)(zrtable[y]);
						zz = (zdata[x] - stz) >> 31;
						MLIB_LOAD_ALPHA8;
						xz = x | zz;
						ux = (x - 1) | zz;
						dx = ((width - x) >> 31) | zz;
						t += dt;
						data = rtable[y | zz];
						zdata =
						    (mlib_s16 *)(zrtable[y |
						    zz]);
						udata = rtable[y | (ux >> 31)];
						ddata = rtable[y | dx];
						dx = (x + 1) | dx;
						MLIB_MAKE_ALPHA8;
						offset =
						    (t >> (MLIB_SHIFT -
						    8)) & 0xFF;
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
						stz = z >> MLIB_SHIFT;
						z += dz;
						zdata =
						    (mlib_s16 *)(zrtable[y]);
						zz = (zdata[x] - stz) >> 31;
						xz = x | zz;
						ux = (x - 1) | zz;
						dx = ((width - x) >> 31) | zz;
						data = rtable[y | zz];
						zdata =
						    (mlib_s16 *)(zrtable[y |
						    zz]);
						udata = rtable[y | (ux >> 31)];
						ddata = rtable[y | dx];
						dx = (x + 1) | dx;
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
						w_uresbg =
						    vis_fpadd16(w_uresc,
						    w_uresbg);
						w_dresbg =
						    vis_fpadd16(w_uresc,
						    w_dresbg);
						zdata[xz] = stz;
						vis_st_u8_i(w_tc, data, xz);
						vis_st_u8_i(w_uresbg, udata,
						    ux);
						vis_st_u8_i(w_dresbg, ddata,
						    dx);
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
	w_div = vis_ld_u8_i(atable, offset + 256);              \
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
	v_bg = *(mlib_f32 *)(data + xz);                         \
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
	*(mlib_s16 *)((mlib_u8 *)zdata + (xz >> 1)) = stz;       \
	*(mlib_f32 *)(data + xz) = vis_fpack16(w_resbg);         \
	*(mlib_f32 *)(udata + ux) = vis_fpack16(w_uresbg);       \
	*(mlib_f32 *)(ddata + dx) = vis_fpack16(w_dresbg);       \
	w_div = vis_fand(w_ff, w_div)

/* *********************************************************** */

void
mlib_GraphicsSpamLineSet_AGZ_32(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 stride,
    mlib_s32 zstride,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 width,
    mlib_s32 height)
{
	mlib_u8 *atable, *udata, *data, *ddata;
	mlib_s16 *zdata, *zdata1;
	mlib_s32 j, x, y, n, t, dt, offset, zz, ux, dx;
	mlib_s32 z, dz, stz, xz, ualpha, dalpha;
	mlib_d64 w_div = 0., w_udiv = 0., w_ddiv = 0.;
	mlib_f32 v_bg = 0.f, v_ubg = 0.f, v_dbg = 0.f;
	mlib_d64 w_c, w_neg = vis_to_double_dup(255);
	mlib_d64 w_c2, w_dc2, w_dc, w_tc;
	mlib_d64 w_const = vis_to_double(0x7FFFFFFF, 0xFFFFFFFF);
	mlib_d64 w_01 = vis_to_double_dup(0x01);
	mlib_d64 w_ff = vis_to_double_dup(0xFF00FF);
	mlib_d64 w_80 = vis_to_double_dup(0x80008000);
	mlib_d64 w_02 = vis_to_double_dup(0x20002);
	mlib_d64 w_ac = vis_to_double(0xFF0000, 0);
	mlib_d64 w_resc, w_uresc, w_dresc, w_resbg, w_uresbg, w_dresbg;

	width *= 4;

	vis_write_gsr((7 << 3) | 7);

	for (; coords < coordsend; coords += MLIB_GDS_NEXT_GZ32) {
		n = coords[MLIB_GDS_LENGTHLINES];
		y = coords[MLIB_GDS_STARTY];
		x = coords[MLIB_GDS_STARTX];
		dt = coords[MLIB_GDS_STEPLINES];
		z = ~coords[MLIB_GDS_STARTZ];
		dz = coords[MLIB_GDS_STEPZ];
		zz = dt >> 31;
		zz = (dt ^ zz) - zz;
		atable =
		    mlib_GraphicsAliasTab_u8 +
		    512 * mlib_GraphicsSqrtTab_u8[zz >> (MLIB_SHIFT - 7)];

		w_c2 = vis_to_double(0, ((mlib_s32 *)coords)[MLIB_GDS_STARTC2]);
		w_c =
		    vis_freg_pair(((mlib_f32 *)coords)[MLIB_GDS_STARTC1],
		    ((mlib_f32 *)coords)[MLIB_GDS_STARTC]);
		w_dc2 = vis_to_double(0, ((mlib_s32 *)coords)[MLIB_GDS_STEPC2]);
		w_dc =
		    vis_freg_pair(((mlib_f32 *)coords)[MLIB_GDS_STEPC1],
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
					offset = (t >> (MLIB_SHIFT - 8)) & 0xFF;

#pragma pipeloop(0)
					for (j = 0; j <= n; j++, x++) {
						mlib_f32 v_tc;

						stz = z >> MLIB_SHIFT;
						z += dz;
						zdata =
						    (mlib_s16
						    *)(MLIB_GET_POINTER(zrtable,
						    t));
						zz = (zdata[x] - stz) >> 31;
						MLIB_LOAD_ALPHA32;
						y = t >> MLIB_SHIFT;
						zdata =
						    (mlib_s16 *)(zrtable[y |
						    zz]);
						data = rtable[y | zz];
						udata = rtable[(y - 1) | zz];
						ddata = rtable[(y + 1) | zz];
						xz = x | zz;
						ux = ((y - 1) >> 31) | xz;
						dx = ((height - y) >> 31) | xz;
						t += dt;
						offset =
						    (t >> (MLIB_SHIFT -
						    8)) & 0xFF;
						xz *= 4;
						ux *= 4;
						dx *= 4;
						MLIB_MAKE_ALPHA32;
					}
				} else {
					zdata = (mlib_s16 *)(zrtable[y =
					    t >> MLIB_SHIFT]);
					ualpha = (y - 1) >> 31;
					dalpha = (height - y) >> 31;
					w_udiv =
					    vis_to_double_dup(2 *
					    MLIB_BG_COEFF);
					w_ddiv = vis_fpsub16(w_neg, w_udiv);
					w_div =
					    vis_to_double_dup(2 *
					    MLIB_BG_COEFF | (MLIB_BG_COEFF <<
					    17) | (MLIB_BG_COEFF << 9));

#pragma pipeloop(0)
					for (j = 0; j <= n; j++, x++) {
						mlib_f32 v_ubg, v_dbg;

						w_tc = vis_faligndata(w_c, w_c);
						w_c = vis_fpadd16(w_c, w_dc);
						w_tc = vis_fand(w_tc, w_ff);
						w_resc =
						    vis_fmul8x16(vis_read_lo
						    (w_div), w_tc);
						w_resc =
						    vis_fpadd16(w_resc, w_ac);

						stz = z >> MLIB_SHIFT;
						z += dz;
						zz = (zdata[x] - stz) >> 31;
						ux = ualpha | zz;
						dx = dalpha | zz;
						data = rtable[y | zz];
						zdata1 =
						    (mlib_s16 *)(zrtable[y |
						    zz]);
						udata = rtable[(y - 1) | ux];
						ddata = rtable[(y + 1) | dx];
						xz = 4 * (x | zz);
						ux = 4 * (x | ux);
						dx = 4 * (x | dx);
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
						w_uresbg =
						    vis_fpadd16(w_resc,
						    w_uresbg);
						w_dresbg =
						    vis_fpadd16(w_resc,
						    w_dresbg);
						*(mlib_s16 *)((mlib_u8 *)zdata1
						    + (xz >> 1)) = stz;
						*(mlib_f32 *)((mlib_u8 *)data +
						    xz) = vis_fpack16(w_tc);
						*(mlib_f32 *)(udata + ux) =
						    vis_fpack16(w_uresbg);
						*(mlib_f32 *)(ddata + dx) =
						    vis_fpack16(w_dresbg);
					}
				}
			} else {
				t = ~x;

				if (dt & (MLIB_ONE - 1)) {
					offset = (t >> (MLIB_SHIFT - 8)) & 0xFF;
					y *= 4;

#pragma pipeloop(0)
					for (j = 0; j <= n; j++, y += 4) {
						mlib_f32 v_tc;

						stz = z >> MLIB_SHIFT;
						z += dz;
						x = (t >> (MLIB_SHIFT -
						    2)) & ~3;
						zdata =
						    *(mlib_s16 **)((mlib_u8 *)
						    zrtable +
						    MLIB_POINTER_MUL(y));
						zz = ((*(mlib_s16 *)((mlib_u8 *)
						    zdata + (x >> 1)) -
						    stz) >> 31) & ~3;
						MLIB_LOAD_ALPHA32;
						ux = (x - 4) | zz;
						dx = (((width -
						    x) >> 31) & ~3) | zz;
						zdata =
						    *(mlib_s16 **)((mlib_u8 *)
						    zrtable +
						    MLIB_POINTER_MUL(y | zz));
						data =
						    *(mlib_u8 **)((mlib_u8 *)
						    rtable +
						    MLIB_POINTER_MUL(y | zz));
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
						xz = x | zz;
						t += dt;
						offset =
						    (t >> (MLIB_SHIFT -
						    8)) & 0xFF;
						MLIB_MAKE_ALPHA32;
					}
				} else {
					y *= 4;
					x = (t >> (MLIB_SHIFT - 2)) & ~3;
					dt = (dt >> (MLIB_SHIFT - 2)) & ~3;
					w_udiv =
					    vis_to_double(0,
					    (dt) ? 2 * MLIB_DG_COEFF : 2 *
					    MLIB_BG_COEFF);
					w_ddiv = vis_fpsub16(w_neg, w_udiv);

#pragma pipeloop(0)
					for (j = 0; j <= n;
					    j++, y += 4, x += dt) {
						mlib_f32 v_ubg, v_dbg, v_tc;

						w_tc = vis_faligndata(w_c, w_c);
						w_c = vis_fpadd16(w_c, w_dc);
						w_tc = vis_fand(w_tc, w_ff);
						v_tc = vis_fpack16(w_tc);
						w_uresc =
						    vis_fmul8x16al(v_tc,
						    vis_read_lo(w_udiv));
						w_uresc =
						    vis_fpadd16(w_uresc, w_ac);
						stz = z >> MLIB_SHIFT;
						z += dz;
						zdata =
						    *(mlib_s16 **)((mlib_u8 *)
						    zrtable +
						    MLIB_POINTER_MUL(y));
						zz = ((*(mlib_s16 *)((mlib_u8 *)
						    zdata + (x >> 1)) -
						    stz) >> 31) & ~3;
						ux = (x - 4) | zz;
						dx = (((width -
						    x) >> 31) & ~3) | zz;
						zdata =
						    *(mlib_s16 **)((mlib_u8 *)
						    zrtable +
						    MLIB_POINTER_MUL(y | zz));
						data =
						    *(mlib_u8 **)((mlib_u8 *)
						    rtable +
						    MLIB_POINTER_MUL(y | zz));
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
						xz = x | zz;
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
						w_uresbg =
						    vis_fpadd16(w_uresc,
						    w_uresbg);
						w_dresbg =
						    vis_fpadd16(w_uresc,
						    w_dresbg);
						*(mlib_s16 *)((mlib_u8 *)zdata +
						    (xz >> 1)) = stz;
						*(mlib_f32 *)((mlib_u8 *)data +
						    xz) = v_tc;
						*(mlib_f32 *)(udata + ux) =
						    vis_fpack16(w_uresbg);
						*(mlib_f32 *)(ddata + dx) =
						    vis_fpack16(w_dresbg);
					}
				}
			}
	}
}

/* *********************************************************** */
