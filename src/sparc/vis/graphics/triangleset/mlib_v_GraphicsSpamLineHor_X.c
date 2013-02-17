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

#pragma ident	"@(#)mlib_v_GraphicsSpamLineHor_X.c	9.2	07/11/05 SMI"

/*
 *  FUNCTIONS
 *    mlib_GraphicsSpamLineHor_X_8  - line filling function for mlib_u8 image
 *    mlib_GraphicsSpamLineHor_X_32 - line filling function for mlib_s32 image
 *
 *  SYNOPSIS
 *    void  mlib_GraphicsSpamLineHor_X_8
 *                                    (mlib_u8  **rtable,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 color,
 *                                     mlib_d64 dcolor);
 *    void  mlib_GraphicsSpamLineHor_X_32
 *                                    (mlib_u8  **rtable,
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
 *                  coords[MLIB_GDS_STARTY + i * MLIB_GDS_NEXT] holds Y
 *                  coords[MLIB_GDS_STARTX + i * MLIB_GDS_NEXT] holds X1
 *                  coords[MLIB_GDS_STOPX  + i * MLIB_GDS_NEXT] holds X2
 *                , functions perform the following filling
 *                  for(j = X1; j < X2; j++) (TYPE*)(rtable[Y])[j] = c
 *    coordsend   for(; coords < coordsend; coords += MLIB_GDS_NEXT)
 *    color       Color to draw, for MLIB_BYTE type of image color must be
 *                  the same in all bytes :
 *                    c  = (color & 0xFF),
 *                    c |= (c << 8),
 *                    c |= (c << 16)
 *    dcolor      dcolor = vis_to_double_dup(color)
 */

#include <vis_proto.h>
#include <mlib_graphics.h>
#include <mlib_GraphicsDrawSet.h>

/* *********************************************************** */

void
mlib_GraphicsSpamLineHor_X_8(
    mlib_u8 **rtable,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 c,
    mlib_d64 dc)
{
	mlib_d64 *wd, wx;
	mlib_u8 *data, *dataend;
	mlib_s32 j, y, xs, xf, emask;

	y = coords[MLIB_GDS_STARTY];

#pragma pipeloop(0)
	for (; coords < coordsend; ) {
		data = rtable[y];
		xs = coords[MLIB_GDS_STARTX];
		xf = coords[MLIB_GDS_STOPX];
		coords += MLIB_GDS_NEXT;
		y = coords[MLIB_GDS_STARTY];

		if (xf - xs > 4) {
			mlib_s32 n = xf - xs - 1;

			dataend = data + xf - 1;
			data = data + xs;
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
		} else
#pragma unroll(1)
			for (j = xs; j < xf; j++)
				data[j] ^= c;
	}
}

/* *********************************************************** */

void
mlib_GraphicsSpamLineHor_X_32(
    mlib_u8 **rtable,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 c,
    mlib_d64 dc)
{
	mlib_d64 wx;
	mlib_s32 *data;
	mlib_s32 j, y, xs, xf;

	y = coords[MLIB_GDS_STARTY];

#pragma pipeloop(0)
	for (; coords < coordsend; ) {
		data = (mlib_s32 *)(rtable[y]);
		xs = coords[MLIB_GDS_STARTX];
		xf = coords[MLIB_GDS_STOPX];
		coords += MLIB_GDS_NEXT;
		y = coords[MLIB_GDS_STARTY];

		if (xf - xs > 4) {
			j = xs;

			if (((mlib_addr)data + 4 * j) & 4)
				data[j++] ^= c;

#pragma pipeloop(0)
			for (; j < xf - 1; j += 2) {
				wx = *((mlib_d64 *)((mlib_u8 *)data + 4 * j));
				wx = vis_fxor(wx, dc);
				*((mlib_d64 *)((mlib_u8 *)data + 4 * j)) = wx;
			}

			if (j < xf)
				data[j] ^= c;
		} else
#pragma pipeloop(0)
			for (j = xs; j < xf; j++)
				data[j] ^= c;
	}
}

/* *********************************************************** */
