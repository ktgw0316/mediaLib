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

#pragma ident	"@(#)mlib_c_GraphicsSpamLineHor_G.c	9.2	07/10/09 SMI"

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

#include <mlib_graphics.h>
#include <mlib_GraphicsDrawSet.h>

/* *********************************************************** */

void
mlib_GraphicsSpamLineHor_G_8(
	mlib_u8 **rtable,
	mlib_s32 *coords,
	mlib_s32 *coordsend)
{
	mlib_u8 *data, *dataend;
	mlib_s32 y, xs, xf, c, dc, c_, stc, stc_;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; coords < coordsend; coords += MLIB_GDS_NEXT_GZ) {
		y = coords[MLIB_GDS_STARTY];
		xs = coords[MLIB_GDS_STARTX];
		xf = coords[MLIB_GDS_STOPX];
		data = rtable[y];
		c = coords[MLIB_GDS_STARTC];
		dc = coords[MLIB_GDS_STEPC];

		dataend = data + xf - 1;
		data += xs;
		c_ = c + dc;
		stc = c >> MLIB_SHIFT;
		dc += dc;

#ifdef __SUNPRO_C
#pragma unroll(1)
#endif /* __SUNPRO_C */
		for (; data <= dataend - 3; data += 4) {
			stc_ = c_ >> MLIB_SHIFT;
			data[0] = stc;
			c += dc;
			stc = c >> MLIB_SHIFT;
			data[1] = stc_;
			c_ += dc;
			stc_ = c_ >> MLIB_SHIFT;
			data[2] = stc;
			c += dc;
			stc = c >> MLIB_SHIFT;
			data[3] = stc_;
			c_ += dc;
		}

		if (data < dataend) {
			stc_ = c_ >> MLIB_SHIFT;
			data[0] = stc;
			c += dc;
			stc = c >> MLIB_SHIFT;
			data[1] = stc_;
			data += 2;
		}

		if (data <= dataend)
			data[0] = stc;
	}
}

/* *********************************************************** */

void
mlib_GraphicsSpamLineHor_G_32(
	mlib_u8 **rtable,
	mlib_s32 *coords,
	mlib_s32 *coordsend)
{
	mlib_u8 *data, *dataend;
	mlib_s32 y, xs, xf, c, dc, c1, dc1, c2, dc2;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; coords < coordsend; coords += MLIB_GDS_NEXT_GZ32) {
		y = coords[MLIB_GDS_STARTY];
		xs = coords[MLIB_GDS_STARTX];
		xf = coords[MLIB_GDS_STOPX];
		data = rtable[y];
		c2 = coords[MLIB_GDS_STARTC2] | (0xFF << (MLIB_SHIFT + 8));
		dc2 = coords[MLIB_GDS_STEPC2];
		c1 = coords[MLIB_GDS_STARTC1];
		dc1 = coords[MLIB_GDS_STEPC1];
		c = coords[MLIB_GDS_STARTC];
		dc = coords[MLIB_GDS_STEPC];

		dataend = data + 4 * xf;
		data += 4 * xs;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; data <= dataend - 8; data += 8) {
#ifdef _LITTLE_ENDIAN
			data[3] = c2 >> (MLIB_SHIFT + 8);
			data[2] = c2 >> MLIB_SHIFT;
			c2 += dc2;
			data[1] = c1 >> MLIB_SHIFT;
			c1 += dc1;
			data[0] = c >> MLIB_SHIFT;
			c += dc;
			data[7] = c2 >> (MLIB_SHIFT + 8);
			data[6] = c2 >> MLIB_SHIFT;
			c2 += dc2;
			data[5] = c1 >> MLIB_SHIFT;
			c1 += dc1;
			data[4] = c >> MLIB_SHIFT;
			c += dc;
#else /* _LITTLE_ENDIAN */
			*(mlib_s16 *)data = c2 >> MLIB_SHIFT;
			c2 += dc2;
			data[2] = c1 >> MLIB_SHIFT;
			c1 += dc1;
			data[3] = c >> MLIB_SHIFT;
			c += dc;
			*(mlib_s16 *)(data + 4) = c2 >> MLIB_SHIFT;
			c2 += dc2;
			data[6] = c1 >> MLIB_SHIFT;
			c1 += dc1;
			data[7] = c >> MLIB_SHIFT;
			c += dc;
#endif /* _LITTLE_ENDIAN */
		}

		if (data < dataend) {
#ifdef _LITTLE_ENDIAN
			data[3] = c2 >> (MLIB_SHIFT + 8);
			data[2] = c2 >> MLIB_SHIFT;
			c2 += dc2;
			data[1] = c1 >> MLIB_SHIFT;
			c1 += dc1;
			data[0] = c >> MLIB_SHIFT;
			c += dc;
#else /* _LITTLE_ENDIAN */
			*(mlib_s16 *)data = c2 >> MLIB_SHIFT;
			c2 += dc2;
			data[2] = c1 >> MLIB_SHIFT;
			c1 += dc1;
			data[3] = c >> MLIB_SHIFT;
			c += dc;
#endif /* _LITTLE_ENDIAN */
		}
	}
}

/* *********************************************************** */
