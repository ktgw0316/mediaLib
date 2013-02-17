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

#pragma ident	"@(#)mlib_v_GraphicsSpamLineHor_Z.c	9.2	07/11/05 SMI"

/*
 *  FUNCTIONS
 *    mlib_GraphicsSpamLineHor_Z_8  - line filling function for mlib_u8 image
 *    mlib_GraphicsSpamLineHor_Z_32 - line filling function for mlib_s32 image
 *
 *  SYNOPSIS
 *    void  mlib_GraphicsSpamLineHor_Z_8
 *                                    (mlib_u8  **rtable,
 *                                     mlib_u8  **zrtable,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 color,
 *                                     mlib_d64 dcolor);
 *    void  mlib_GraphicsSpamLineHor_Z_32
 *                                    (mlib_u8  **rtable,
 *                                     mlib_u8  **zrtable,
 *                                     mlib_s32 *coords,
 *                                     mlib_s32 *coordsend,
 *                                     mlib_s32 color,
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
 *    dcolor      dcolor = vis_to_double_dup(color)
 */

#include <vis_proto.h>
#include <mlib_graphics.h>
#include <mlib_GraphicsDrawSet.h>

/* *********************************************************** */

#define	MLIB_CYCLE_Z                                            \
	for (j = xs; j < xf; j++, z += dz)                      \
	    if ((stz) > zz) {                                   \
		zz = zdata[1];                                  \
		zdata++;                                        \
		stz = z >> MLIB_SHIFT;                          \
	    } else {                                            \
		zz = zdata[1];                                  \
		*zdata = stz;                                   \
		zdata++;                                        \
		stz = z >> MLIB_SHIFT;                          \
		data[j] = c;                                    \
	    }

/* *********************************************************** */

#define	MLIB_LOAD_Z                                             \
	z = coords[MLIB_GDS_STARTZ];                            \
	dz = coords[MLIB_GDS_STEPZ];                            \
	zdata += xs;                                            \
	zz = *zdata;                                            \
	stz = z >> MLIB_SHIFT;                                  \
	z += dz

/* *********************************************************** */

#define	MLIB_LOAD_XZ(MLIB_TYPE)                                 \
	y = coords[MLIB_GDS_STARTY];                            \
	xs = coords[MLIB_GDS_STARTX];                           \
	xf = coords[MLIB_GDS_STOPX];                            \
	data = (MLIB_TYPE *) (rtable[y]);                       \
	zdata = (mlib_s16 *)(zrtable[y])

/* *********************************************************** */

void
mlib_GraphicsSpamLineHor_Z_8(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 c,
    mlib_d64 dc)
{
	mlib_u8 *data;
	mlib_s16 *zdata;
	mlib_s32 j, y, xs, xf, z, dz, stz, zz;

	if (((mlib_addr)rtable[0] | (mlib_addr)zrtable[0] | (mlib_addr)rtable[1]
	    | (mlib_addr)zrtable[1]) & 7)
#pragma pipeloop(0)
		for (; coords < coordsend; coords += MLIB_GDS_NEXT_GZ) {
			MLIB_LOAD_XZ(mlib_u8);

			MLIB_LOAD_Z;

#pragma pipeloop(0)
			MLIB_CYCLE_Z;
	} else {
		vis_write_gsr(1 << 3);

		for (; coords < coordsend; coords += MLIB_GDS_NEXT_GZ) {
			MLIB_LOAD_XZ(mlib_u8);

			if (xf - xs < 8) {
				MLIB_LOAD_Z;

#pragma pipeloop(0)
				MLIB_CYCLE_Z;
			} else {
				mlib_s32 emask = 0xFF, xsa = xs & ~7;
				mlib_s32 emask0, emask1;
				mlib_d64 *pwz = (mlib_d64 *)(zdata + xsa);
				mlib_d64 w_z0, w_z1, w_stz0, w_stz1;
				mlib_d64 w_sz, w_tz0, w_tz1, w_tz2, w_tz3;

				z = coords[MLIB_GDS_STARTZ];
				dz = coords[MLIB_GDS_STEPZ];
				w_z0 = pwz[0];
				w_z1 = pwz[1];
				z -= (xs - xsa) * dz;

				emask >>= (xs - xsa);
				emask0 = emask >> 4;
				emask1 = emask & 0xF;
				w_tz0 = vis_to_double(z, z + dz);
				w_sz = vis_to_double_dup(dz + dz);
				w_tz1 = vis_fpadd32(w_tz0, w_sz);
				w_sz = vis_fpadd32(w_sz, w_sz);

				w_stz0 = vis_fpackfix_pair(w_tz0, w_tz1);
				w_tz2 = vis_fpadd32(w_tz0, w_sz);
				w_tz3 = vis_fpadd32(w_tz1, w_sz);
				emask0 &= vis_fcmple16(w_stz0, w_z0);
				w_stz1 = vis_fpackfix_pair(w_tz2, w_tz3);
				w_sz = vis_fpadd32(w_sz, w_sz);
				emask1 &= vis_fcmple16(w_stz1, w_z1);

#pragma unroll(1)
				for (j = xsa; j < xf - 7; j += 8) {
					w_tz0 = vis_fpadd32(w_tz0, w_sz);
					w_tz1 = vis_fpadd32(w_tz1, w_sz);
					w_tz2 = vis_fpadd32(w_tz2, w_sz);
					w_tz3 = vis_fpadd32(w_tz3, w_sz);
					w_z0 = pwz[2];
					w_z1 = pwz[3];
					vis_pst_16(w_stz0, pwz++, emask0);
					vis_pst_16(w_stz1, pwz++, emask1);
					vis_pst_8(dc, data + j,
					    (emask0 << 4) | emask1);
					w_stz0 =
					    vis_fpackfix_pair(w_tz0, w_tz1);
					w_stz1 =
					    vis_fpackfix_pair(w_tz2, w_tz3);
					emask0 = vis_fcmple16(w_stz0, w_z0);
					emask1 = vis_fcmple16(w_stz1, w_z1);
				}

				if (j < xf) {
					emask = (emask0 << 4) | emask1;
					emask &= (0xFF << (8 - xf + j));
					emask0 = emask >> 4;
					vis_pst_16(w_stz0, pwz++, emask0);
					vis_pst_16(w_stz1, pwz, emask);
					vis_pst_8(dc, data + j, emask);
				}
			}
		}
	}
}

/* *********************************************************** */

void
mlib_GraphicsSpamLineHor_Z_32(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 c,
    mlib_d64 dc)
{
	mlib_s32 *data;
	mlib_s16 *zdata;
	mlib_s32 j, y, xs, xf, z, dz, stz, zz;

	if (((mlib_addr)rtable[0] | (mlib_addr)zrtable[0] | (mlib_addr)rtable[1]
	    | (mlib_addr)zrtable[1]) & 7)
		for (; coords < coordsend; coords += MLIB_GDS_NEXT_GZ) {
			MLIB_LOAD_XZ(mlib_s32);

			MLIB_LOAD_Z;

#pragma pipeloop(0)
			MLIB_CYCLE_Z;
	} else {
		vis_write_gsr(1 << 3);

		for (; coords < coordsend; coords += MLIB_GDS_NEXT_GZ) {
			MLIB_LOAD_XZ(mlib_s32);

#pragma pipeloop(0)

			if (xf - xs < 8) {
				MLIB_LOAD_Z;

#pragma pipeloop(0)
				MLIB_CYCLE_Z;
			} else {
				mlib_s32 xsa = xs & ~3;
				mlib_s32 emask0 = 0xF, emask1 = 0xF;
				mlib_d64 *pwz = (mlib_d64 *)(zdata + xsa);
				mlib_d64 *pwx = (mlib_d64 *)(data + xsa);
				mlib_d64 w_z0, w_z1, w_stz0, w_stz1;
				mlib_d64 w_sz, w_tz0, w_tz1, w_tz2, w_tz3;

				z = coords[MLIB_GDS_STARTZ];
				dz = coords[MLIB_GDS_STEPZ];
				w_z0 = pwz[0];
				w_z1 = pwz[1];
				z -= (xs - xsa) * dz;

				emask0 = emask0 >> (xs - xsa);
				w_tz0 = vis_to_double(z, z + dz);
				w_sz = vis_to_double_dup(dz + dz);
				w_tz1 = vis_fpadd32(w_tz0, w_sz);
				w_sz = vis_fpadd32(w_sz, w_sz);

				w_stz0 = vis_fpackfix_pair(w_tz0, w_tz1);
				w_tz2 = vis_fpadd32(w_tz0, w_sz);
				w_tz3 = vis_fpadd32(w_tz1, w_sz);
				emask0 &= vis_fcmple16(w_stz0, w_z0);
				w_stz1 = vis_fpackfix_pair(w_tz2, w_tz3);
				w_sz = vis_fpadd32(w_sz, w_sz);
				emask1 = vis_fcmple16(w_stz1, w_z1);

#pragma unroll(1)
				for (j = xsa; j < xf - 7; j += 8) {
					w_tz0 = vis_fpadd32(w_tz0, w_sz);
					w_tz1 = vis_fpadd32(w_tz1, w_sz);
					w_tz2 = vis_fpadd32(w_tz2, w_sz);
					w_tz3 = vis_fpadd32(w_tz3, w_sz);
					w_z0 = pwz[2];
					w_z1 = pwz[3];
					vis_pst_16(w_stz0, pwz++, emask0);
					vis_pst_16(w_stz1, pwz++, emask1);
					vis_pst_32(dc, pwx++, emask0 >> 2);
					vis_pst_32(dc, pwx++, emask0);
					vis_pst_32(dc, pwx++, emask1 >> 2);
					vis_pst_32(dc, pwx++, emask1);
					w_stz0 =
					    vis_fpackfix_pair(w_tz0, w_tz1);
					w_stz1 =
					    vis_fpackfix_pair(w_tz2, w_tz3);
					emask0 = vis_fcmple16(w_stz0, w_z0);
					emask1 = vis_fcmple16(w_stz1, w_z1);
				}

				if (j < xf) {
					mlib_s32 emask = (emask0 << 4) | emask1;

					emask &= (0xFF << (8 - xf + j));
					emask0 = emask >> 4;
					emask1 = emask & 0xF;
					vis_pst_16(w_stz0, pwz++, emask0);
					vis_pst_16(w_stz1, pwz, emask1);
					vis_pst_32(dc, pwx++, emask0 >> 2);
					vis_pst_32(dc, pwx++, emask0);
					vis_pst_32(dc, pwx++, emask1 >> 2);
					vis_pst_32(dc, pwx++, emask1);
				}
			}
		}
	}
}

/* *********************************************************** */
