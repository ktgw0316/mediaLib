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

#pragma ident	"@(#)mlib_v_GraphicsSpamLineHor_BZ.c	9.2	07/11/05 SMI"

/*
 *  FUNCTIONS
 *    mlib_GraphicsSpamLineHor_BZ_8  - line filling function for mlib_u8 image
 *				       with alpha blending
 *    mlib_GraphicsSpamLineHor_BZ_32 - line filling function for mlib_s32 image
 *				       with alpha blending
 *
 *  SYNOPSIS
 *    void  mlib_GraphicsSpamLineHor_BZ_8
 *                                    (mlib_u8  **rtable,
 *                                     mlib_u8  **zrtable,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 color,
 *                                     mlib_s32 alpha,
 *                                     mlib_d64 dcolor);
 *    void  mlib_GraphicsSpamLineHor_BZ_32
 *                                    (mlib_u8  **rtable,
 *                                     mlib_u8  **zrtable,
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
 *    zrtable     Pointer to array of starts of row of zbuffer, zrtable[-1][-1]
 *                  is buffer for unwanted stores
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
#include <mlib_GraphicsBlend.h>
#include <mlib_GraphicsDrawSet.h>

/* *********************************************************** */

void
mlib_GraphicsSpamLineHor_BZ_8(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 c,
    mlib_s32 a,
    mlib_d64 dc)
{
	mlib_u8 *data;
	mlib_s16 *zdata;
	mlib_s32 j, y, xs, xf, z, dz, stz, zz;

	MLIB_GRAPHICS_DECL_VAR_B_8;
	MLIB_GRAPHICS_SET_VAR_B_8(c, a);
	xs = dc;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; coords < coordsend; coords += MLIB_GDS_NEXT_GZ) {
		y = coords[MLIB_GDS_STARTY];
		xs = coords[MLIB_GDS_STARTX];
		xf = coords[MLIB_GDS_STOPX];
		data = rtable[y];
		zdata = (mlib_s16 *)(zrtable[y]) + xs;
		z = coords[MLIB_GDS_STARTZ];
		dz = coords[MLIB_GDS_STEPZ];

		zz = *zdata;
		stz = z >> MLIB_SHIFT;
		z += dz;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = xs; j < xf; j++, z += dz)
			if ((stz) > zz) {
				zz = zdata[1];
				zdata++;
				stz = z >> MLIB_SHIFT;
			} else {
				zz = zdata[1];
				*zdata = stz;
				zdata++;
				stz = z >> MLIB_SHIFT;
				MLIB_GRAPHICS_BLEND_8(data + j)
			}
	}
}

/* *********************************************************** */

void
mlib_GraphicsSpamLineHor_BZ_32(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 c,
    mlib_s32 a,
    mlib_d64 dc)
{
	mlib_s32 *data;
	mlib_s16 *zdata;
	mlib_s32 j, y, xs, xf, z, dz, stz, zz;

	MLIB_GRAPHICS_DECL_VAR_B_32;
	MLIB_GRAPHICS_SET_VAR_B_32(c, a);
	xs = dc;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; coords < coordsend; coords += MLIB_GDS_NEXT_GZ) {
		y = coords[MLIB_GDS_STARTY];
		xs = coords[MLIB_GDS_STARTX];
		xf = coords[MLIB_GDS_STOPX];
		data = (mlib_s32 *)(rtable[y]);
		zdata = (mlib_s16 *)(zrtable[y]) + xs;
		z = coords[MLIB_GDS_STARTZ];
		dz = coords[MLIB_GDS_STEPZ];
		zz = *zdata;
		stz = z >> MLIB_SHIFT;
		z += dz;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = xs; j < xf; j++, z += dz)
			if ((stz) > zz) {
				zz = zdata[1];
				zdata++;
				stz = z >> MLIB_SHIFT;
			} else {
				zz = zdata[1];
				*zdata = stz;
				zdata++;
				stz = z >> MLIB_SHIFT;
				MLIB_GRAPHICS_BLEND_32(data + j);
			}
	}
}

/* *********************************************************** */
