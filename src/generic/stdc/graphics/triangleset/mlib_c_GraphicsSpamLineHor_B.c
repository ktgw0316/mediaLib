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

#pragma ident	"@(#)mlib_c_GraphicsSpamLineHor_B.c	9.2	07/10/09 SMI"

/*
 *  FUNCTIONS
 *    mlib_GraphicsSpamLineHor_B_8  - line filling function for mlib_u8 image
 *					with alpha blending
 *    mlib_GraphicsSpamLineHor_B_32 - line filling function for mlib_s32 image
 *					with alpha blending
 *
 *  SYNOPSIS
 *    void  mlib_GraphicsSpamLineHor_B_8
 *                                    (mlib_u8  **rtable,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 color,
 *                                     mlib_s32 alpha,
 *                                     mlib_d64 dcolor);
 *    void  mlib_GraphicsSpamLineHor_B_32
 *                                    (mlib_u8  **rtable,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 color,
 *                                     mlib_s32 alpha,
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
 *    alpha       Alpha blending value.
 */

#include <mlib_graphics.h>
#include <mlib_GraphicsDrawSet.h>
#include <mlib_GraphicsBlend.h>

/* *********************************************************** */

void
mlib_GraphicsSpamLineHor_B_8(
	mlib_u8 **rtable,
	mlib_s32 *coords,
	mlib_s32 *coordsend,
	mlib_s32 c,
	mlib_s32 a,
	mlib_d64 dc)
{
	mlib_u8 *data, *dataend;
	mlib_s32 y, xs, xf;

	MLIB_GRAPHICS_DECL_VAR_B_8;
	MLIB_GRAPHICS_SET_VAR_B_8(c, a);

	y = coords[MLIB_GDS_STARTY];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; coords < coordsend - MLIB_GDS_NEXT; ) {
		data = rtable[y];
		xs = coords[MLIB_GDS_STARTX];
		xf = coords[MLIB_GDS_STOPX];
		coords += MLIB_GDS_NEXT;
		y = coords[MLIB_GDS_STARTY];
		dataend = data + xf;
		data = data + xs;

		for (; data < dataend; data++)
			MLIB_GRAPHICS_BLEND_8(data)
	}

	if (coords < coordsend) {
		data = rtable[y];
		xs = coords[MLIB_GDS_STARTX];
		xf = coords[MLIB_GDS_STOPX];
		dataend = data + xf;
		data = data + xs;

		for (; data < dataend; data++)
			MLIB_GRAPHICS_BLEND_8(data)
	}
}

/* *********************************************************** */

void
mlib_GraphicsSpamLineHor_B_32(
	mlib_u8 **rtable,
	mlib_s32 *coords,
	mlib_s32 *coordsend,
	mlib_s32 c,
	mlib_s32 a,
	mlib_d64 dc)
{
	mlib_s32 *data, *dataend;
	mlib_s32 y, xs, xf;

	MLIB_GRAPHICS_DECL_VAR_B_32;
	MLIB_GRAPHICS_SET_VAR_B_32(c, a);

	y = coords[MLIB_GDS_STARTY];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; coords < coordsend - MLIB_GDS_NEXT; ) {
		data = (mlib_s32 *)(rtable[y]);
		xs = coords[MLIB_GDS_STARTX];
		xf = coords[MLIB_GDS_STOPX];
		coords += MLIB_GDS_NEXT;
		y = coords[MLIB_GDS_STARTY];
		dataend = data + xf;
		data = data + xs;

		for (; data < dataend; data ++)
			MLIB_GRAPHICS_BLEND_32(data)
	}

	if (coords < coordsend) {
		data = (mlib_s32 *)(rtable[y]);
		xs = coords[MLIB_GDS_STARTX];
		xf = coords[MLIB_GDS_STOPX];
		dataend = data + xf;
		data = data + xs;

		for (; data < dataend; data ++)
			MLIB_GRAPHICS_BLEND_32(data)

	}
}

/* *********************************************************** */
