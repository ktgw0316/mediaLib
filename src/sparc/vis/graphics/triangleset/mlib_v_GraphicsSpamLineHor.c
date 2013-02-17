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

#pragma ident	"@(#)mlib_v_GraphicsSpamLineHor.c	9.2	07/11/05 SMI"

/*
 *  FUNCTIONS
 *    mlib_GraphicsSpamLineHor_8  - line filling function for mlib_u8 image
 *    mlib_GraphicsSpamLineHor_32 - line filling function for mlib_s32 image
 *
 *  SYNOPSIS
 *    void  mlib_GraphicsSpamLineHor_8
 *                                    (mlib_u8  **rtable,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 color,
 *                                     mlib_d64 dcolor);
 *    void  mlib_GraphicsSpamLineHor_32
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
mlib_GraphicsSpamLineHor_8(
    mlib_u8 **rtable,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 c,
    mlib_d64 dc)
{
	mlib_u8 *data, *data1, *dataend;
	mlib_s32 j, y, xs, xf;

	y = coords[MLIB_GDS_STARTY];

#pragma pipeloop(0)
	for (; coords < coordsend; ) {
		data = rtable[y];
		xs = coords[MLIB_GDS_STARTX];
		xf = coords[MLIB_GDS_STOPX];
		coords += MLIB_GDS_NEXT;
		y = coords[MLIB_GDS_STARTY];

		if (xf - xs > 8) {
			mlib_s32 k = xf - xs - 1, mask;

			dataend = data + xf;
			data += xs;
			data1 = (mlib_u8 *)((mlib_addr)data & ~7);
			mask = vis_edge8(data, dataend);

			if (k >= 0)
				vis_pst_8(dc, data1, mask);
			data = data1 + 8;
			for (; data < dataend - 7; data += 8)
				*(mlib_d64 *)data = dc;

			if (data < dataend)
				vis_pst_8(dc, data, vis_edge8(data,
				    dataend - 1));
		} else
#pragma unroll(1)
			for (j = xs; j < xf; j++)
				data[j] = c;
	}
}

/* *********************************************************** */

void
mlib_GraphicsSpamLineHor_32(
    mlib_u8 **rtable,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 c,
    mlib_d64 dc)
{
	mlib_s32 *data, *data1, *dataend;
	mlib_s32 y, xs, xf;

	y = coords[MLIB_GDS_STARTY];

#pragma pipeloop(0)
	for (; coords < coordsend; ) {
		data = (mlib_s32 *)(rtable[y]);
		xs = coords[MLIB_GDS_STARTX];
		xf = coords[MLIB_GDS_STOPX];
		coords += MLIB_GDS_NEXT;
		y = coords[MLIB_GDS_STARTY];
		dataend = data + xf;
		data = data + xs;
		data1 = (mlib_s32 *)(((mlib_addr)data + 7) & ~7);

		if (data1 < dataend) {
			if (data < data1)
				(*data++) = c;
			data1 = (mlib_s32 *)((mlib_addr)dataend & ~7);
			for (; data < data1; data += 2)
				*(mlib_d64 *)data = dc;
		}

		if (data < dataend)
			*data = c;
	}
}

/* *********************************************************** */
