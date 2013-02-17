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

#pragma ident	"@(#)mlib_c_GraphicsSpamLineHor_BG.c	9.2	07/10/09 SMI"

/*
 *  FUNCTIONS
 *    mlib_GraphicsSpamLineHor_BG_8  - line filling function for mlib_u8 image
 *    mlib_GraphicsSpamLineHor_BG_32 - line filling function for mlib_s32 image
 *
 *  SYNOPSIS
 *    void  mlib_GraphicsSpamLineHor_BG_[8 | 32]
 *                                    (mlib_u8  **rtable,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *				       mlib_s32 alpha);
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
 *    alpha       Alpha blending value.
 */

#include <mlib_graphics.h>
#include <mlib_GraphicsDrawSet.h>
#include <mlib_ImageDivTables.h>

/* *********************************************************** */

#define	MLIB_BLENDING_8(s, c)                                   \
	s = (mlib_U82D64[s] * A0) + (mlib_U82D64[c] * A1)

/* *********************************************************** */

void
mlib_GraphicsSpamLineHor_BG_8(
    mlib_u8 **rtable,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 alpha)
{
	mlib_u8 *data, *dataend;
	mlib_s32 y, xs, xf, c, dc, c_, stc, stc_, dstc, dstc_;
	mlib_d64 A0, A1;

	A1 = (alpha & 0xFF) / 255.;
	A0 = 1. - A1;

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
		dstc = data[0];
		MLIB_BLENDING_8(dstc, stc);
		dc += dc;

#ifdef __SUNPRO_C
#pragma unroll(1)
#endif /* __SUNPRO_C */

		for (; data <= dataend - 3; data += 4) {
			stc_ = c_ >> MLIB_SHIFT;
			dstc_ = data[1];
			MLIB_BLENDING_8(dstc_, stc_);
			data[0] = dstc;
			c += dc;
			stc = c >> MLIB_SHIFT;
			dstc = data[2];
			MLIB_BLENDING_8(dstc, stc);
			data[1] = dstc_;
			c_ += dc;
			stc_ = c_ >> MLIB_SHIFT;
			dstc_ = data[3];
			MLIB_BLENDING_8(dstc_, stc_);
			data[2] = dstc;
			c += dc;
			stc = c >> MLIB_SHIFT;
			dstc = data[4];
			MLIB_BLENDING_8(dstc, stc);
			data[3] = dstc_;
			c_ += dc;
		}

		if (data < dataend) {
			stc_ = c_ >> MLIB_SHIFT;
			dstc_ = data[1];
			MLIB_BLENDING_8(dstc_, stc_);
			data[0] = dstc;
			c += dc;
			stc = c >> MLIB_SHIFT;
			dstc = data[2];
			MLIB_BLENDING_8(dstc, stc);
			data[1] = dstc_;
			data += 2;
		}

		if (data <= dataend)
			data[0] = dstc;
	}
}

/* *********************************************************** */

#define	MLIB_PUT_BG_32(DATA)                                             \
	src = *((mlib_s32 *)(DATA));                                     \
	b = (src >> 16) & 0xFF;                                          \
	g = (src >> 8) & 0xFF;                                           \
	r = src & 0xFF;                                                  \
	MLIB_BLENDING_8(b, (c2 >> MLIB_SHIFT) & 0xFF);                   \
	MLIB_BLENDING_8(g, (c1 >> MLIB_SHIFT));                          \
	MLIB_BLENDING_8(r, (c >> MLIB_SHIFT));                           \
	*((mlib_s32 *)(DATA)) = 0xFF000000 | (b << 16 | (g << 8) | r);   \
	c2 += dc2;                                                       \
	c1 += dc1;                                                       \
	c += dc;

/* *********************************************************** */

void
mlib_GraphicsSpamLineHor_BG_32(
    mlib_u8 **rtable,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 alpha)
{
	mlib_u8 *data, *dataend;
	mlib_s32 y, xs, xf, c, dc, c1, dc1, c2, dc2, r, g, b, src;
	mlib_d64 A0, A1;

	A1 = alpha / 255.;
	A0 = 1. - A1;

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
			data[3] = 0xFF;
			MLIB_BLENDING_8(data[2], ((c2 >> MLIB_SHIFT) & 0xFF));
			c2 += dc2;
			MLIB_BLENDING_8(data[1], (c1 >> MLIB_SHIFT));
			c1 += dc1;
			MLIB_BLENDING_8(data[0], (c >> MLIB_SHIFT));
			c += dc;
			data[7] = 0xFF;
			MLIB_BLENDING_8(data[6], ((c2 >> MLIB_SHIFT) & 0xFF));
			c2 += dc2;
			MLIB_BLENDING_8(data[5], (c1 >> MLIB_SHIFT));
			c1 += dc1;
			MLIB_BLENDING_8(data[4], (c >> MLIB_SHIFT));
			c += dc;
#else /* _LITTLE_ENDIAN */
			MLIB_PUT_BG_32(data)
			    MLIB_PUT_BG_32(data + 4)
#endif /* _LITTLE_ENDIAN */
		}

		if (data < dataend) {
#ifdef _LITTLE_ENDIAN
			data[3] = 0xFF;
			MLIB_BLENDING_8(data[2], ((c2 >> MLIB_SHIFT) & 0xFF));
			c2 += dc2;
			MLIB_BLENDING_8(data[1], (c1 >> MLIB_SHIFT));
			c1 += dc1;
			MLIB_BLENDING_8(data[0], (c >> MLIB_SHIFT));
			c += dc;
#else /* _LITTLE_ENDIAN */
			MLIB_PUT_BG_32(data)
#endif /* _LITTLE_ENDIAN */
		}
	}
}

/* *********************************************************** */
