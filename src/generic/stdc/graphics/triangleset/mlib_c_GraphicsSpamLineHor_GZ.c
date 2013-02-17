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

#pragma ident	"@(#)mlib_c_GraphicsSpamLineHor_GZ.c	9.2	07/10/09 SMI"

/*
 *  FUNCTIONS
 *    mlib_GraphicsSpamLineHor_GZ_8  - line filling function for mlib_u8 image
 *    mlib_GraphicsSpamLineHor_GZ_32 - line filling function for mlib_s32 image
 *
 *  SYNOPSIS
 *    void  mlib_GraphicsSpamLineHor_GZ_8
 *                                    (mlib_u8  **rtable,
 *                                     mlib_u8  **zrtable,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend);
 *    void  mlib_GraphicsSpamLineHor_GZ_32
 *                                    (mlib_u8  **rtable,
 *                                     mlib_u8  **zrtable,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend);
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
 */

#include <mlib_graphics.h>
#include <mlib_GraphicsDrawSet.h>

/* *********************************************************** */

void
mlib_GraphicsSpamLineHor_GZ_8(
	mlib_u8 **rtable,
	mlib_u8 **zrtable,
	mlib_s32 *coords,
	mlib_s32 *coordsend)
{
	mlib_u8 *data;
	mlib_s16 *zdata;
	mlib_s32 j, y, xs, xf, z, dz, stz, zz;
	mlib_s32 c, dc;

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
		c = coords[MLIB_GDS_STARTC];
		dc = coords[MLIB_GDS_STEPC];
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
				c += dc;
			} else {
				zz = zdata[1];
				*zdata = stz;
				zdata++;
				stz = z >> MLIB_SHIFT;
				data[j] = c >> MLIB_SHIFT;
				c += dc;
			}
	}
}

/* *********************************************************** */

void
mlib_GraphicsSpamLineHor_GZ_32(
	mlib_u8 **rtable,
	mlib_u8 **zrtable,
	mlib_s32 *coords,
	mlib_s32 *coordsend)
{
	mlib_u8 *data;
	mlib_s16 *zdata;
	mlib_s32 j, y, xs, xf, z, dz, stz, zz;
	mlib_s32 c, dc, c1, dc1, c2, dc2;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; coords < coordsend; coords += MLIB_GDS_NEXT_GZ32) {
		y = coords[MLIB_GDS_STARTY];
		xs = coords[MLIB_GDS_STARTX];
		xf = coords[MLIB_GDS_STOPX];
		data = rtable[y];
		zdata = (mlib_s16 *)(zrtable[y]) + xs;
		z = coords[MLIB_GDS_STARTZ];
		dz = coords[MLIB_GDS_STEPZ];
		c2 = coords[MLIB_GDS_STARTC2] | (0xFF << (MLIB_SHIFT + 8));
		dc2 = coords[MLIB_GDS_STEPC2];
		c1 = coords[MLIB_GDS_STARTC1];
		dc1 = coords[MLIB_GDS_STEPC1];
		c = coords[MLIB_GDS_STARTC];
		dc = coords[MLIB_GDS_STEPC];
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
				c2 += dc2;
				c1 += dc1;
				c += dc;
			} else {
				mlib_u8 *data1 = data + 4 * j;

				zz = zdata[1];
				*zdata = stz;
				zdata++;
				stz = z >> MLIB_SHIFT;
#ifdef _LITTLE_ENDIAN
				data1[3] = c2 >> (MLIB_SHIFT + 8);
				data1[2] = c2 >> MLIB_SHIFT;
				c2 += dc2;
				data1[1] = c1 >> MLIB_SHIFT;
				c1 += dc1;
				data1[0] = c >> MLIB_SHIFT;
				c += dc;
#else /* _LITTLE_ENDIAN */
				*(mlib_s16 *)data1 = c2 >> MLIB_SHIFT;
				c2 += dc2;
				data1[2] = c1 >> MLIB_SHIFT;
				c1 += dc1;
				data1[3] = c >> MLIB_SHIFT;
				c += dc;
#endif /* _LITTLE_ENDIAN */
			}
	}
}

/* *********************************************************** */
