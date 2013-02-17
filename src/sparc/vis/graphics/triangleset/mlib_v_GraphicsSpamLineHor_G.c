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

#pragma ident	"@(#)mlib_v_GraphicsSpamLineHor_G.c	9.2	07/11/05 SMI"

/*
 *  FUNCTIONS
 *    mlib_GraphicsSpamLineHor_G_8  - line filling function for mlib_u8 image
 *    mlib_GraphicsSpamLineHor_G_32 - line filling function for mlib_s32 image
 *
 *  SYNOPSIS
 *    void  mlib_GraphicsSpamLineHor_G_[8 | 32]
 *                                    (mlib_u8  **rtable,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend);
 *
 *  ARGUMENTS
 *    rtable      Pointer to array of starts of row, rtable[-1][-1] is buffer
 *                  for unwanted stores
 *    coords      Array of values sets using in *Spam* functions:
 *                  coords[MLIB_GDS_STARTY + i * MLIB_GDS_NEXT] holds Y
 *                  coords[MLIB_GDS_STARTX + i * MLIB_GDS_NEXT] holds X1
 *                  coords[MLIB_GDS_STOPX  + i * MLIB_GDS_NEXT] holds X2
 *                , functions perform the following filling
 *                  for(j = X1; j < X2; j++) (TYPE*)(rtable[Y])[j] = c
 *    coordsend   for(; coords < coordsend; coords += MLIB_GDS_NEXT)
 */

#include <vis_proto.h>
#include <mlib_graphics.h>
#include <mlib_GraphicsDrawSet.h>

/* *********************************************************** */

void
mlib_GraphicsSpamLineHor_G_8(
    mlib_u8 **rtable,
    mlib_s32 *coords,
    mlib_s32 *coordsend)
{
	mlib_u8 *data, *data1, *dataend;
	mlib_s32 y, xs, xf, c, dc;

	vis_write_gsr(7);

#pragma pipeloop(0)
	for (; coords < coordsend; coords += MLIB_GDS_NEXT_GZ) {
		y = coords[MLIB_GDS_STARTY];
		xs = coords[MLIB_GDS_STARTX];
		xf = coords[MLIB_GDS_STOPX];
		data = rtable[y];
		c = coords[MLIB_GDS_STARTC];
		dc = coords[MLIB_GDS_STEPC];

		dataend = data + xf;
		data += xs;
		data1 = (mlib_u8 *)(((mlib_addr)data + 3) & ~3);

		if (data1 < (mlib_u8 *)((mlib_addr)dataend & ~7)) {
			mlib_d64 w_c, w_dc;
			mlib_s32 tc0, tc1, tc2, tc3;

			for (; data < data1; data++) {
				*data = c >> MLIB_SHIFT;
				c += dc;
			}

			data1 = (mlib_u8 *)((mlib_addr)dataend & ~7);

			tc0 = (c << (23 - MLIB_SHIFT)) & ~0xFFFF;
			tc1 = (c + dc) >> (MLIB_SHIFT - 7);
			tc2 = ((c + 2 * dc) << (23 - MLIB_SHIFT)) & ~0xFFFF;
			tc3 = (c + 3 * dc) >> (MLIB_SHIFT - 7);
			w_c = vis_to_double(tc0 | tc1, tc2 | tc3);
			tc0 = dc >> (MLIB_SHIFT - 9);
			w_dc = vis_to_double_dup((tc0 & 0xFFFF) | (tc0 << 16));

			for (; data < data1; data += 4) {
				*(mlib_f32 *)data = vis_fpack16(w_c);
				w_c = vis_fpadd16(w_c, w_dc);
			}

			vis_pst_8(vis_fpack16_pair(w_c, vis_fpadd16(w_c, w_dc)),
			    data, 0xFF << (data - dataend + 8));
		} else
#pragma unroll(1)
			for (; data < dataend; data++) {
				*data = c >> MLIB_SHIFT;
				c += dc;
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
mlib_GraphicsSpamLineHor_G_32(
    mlib_u8 **rtable,
    mlib_s32 *coords,
    mlib_s32 *coordsend)
{
	mlib_d64 w_c, w_c2, w_dc, w_dc2;
	mlib_f32 v_t, v_zero = vis_to_float(0);
	mlib_f32 v_7f = vis_to_float(0x7FFFFFFF);
	mlib_u8 *data;
	mlib_s32 j, y, xs, xf;

#pragma pipeloop(0)
	for (; coords < coordsend; coords += MLIB_GDS_NEXT_GZ32) {
		vis_write_gsr(8 << 3);

		y = coords[MLIB_GDS_STARTY];
		xs = coords[MLIB_GDS_STARTX];
		xf = coords[MLIB_GDS_STOPX];
		data = rtable[y];

		w_c2 =
		    vis_freg_pair(v_7f, ((mlib_f32 *)coords)[MLIB_GDS_STARTC2]);
		w_c =
		    vis_freg_pair(((mlib_f32 *)coords)[MLIB_GDS_STARTC1],
		    ((mlib_f32 *)coords)[MLIB_GDS_STARTC]);
		w_dc2 =
		    vis_freg_pair(v_zero,
		    ((mlib_f32 *)coords)[MLIB_GDS_STEPC2]);
		w_dc =
		    vis_freg_pair(((mlib_f32 *)coords)[MLIB_GDS_STEPC1],
		    ((mlib_f32 *)coords)[MLIB_GDS_STEPC]);
		w_c = vis_fpackfix_pair(w_c2, w_c);
		w_dc = vis_fpackfix_pair(w_dc2, w_dc);

		vis_write_gsr(0);

		v_t = vis_fpack16(w_c);
		vis_fpadd16(w_c, w_dc);

		for (j = xs; j < xf - 1; j += 2) {
			MLIB_PUT_G_32(data, 4 * j);
			MLIB_PUT_G_32(data, 4 * (j + 1));
		}

		if (j < xf) {
			MLIB_PUT_G_32(data, 4 * j);
		}
	}
}

/* *********************************************************** */
