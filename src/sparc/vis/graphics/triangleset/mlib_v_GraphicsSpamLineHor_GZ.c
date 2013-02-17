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

#pragma ident	"@(#)mlib_v_GraphicsSpamLineHor_GZ.c	9.2	07/11/05 SMI"

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

#include <vis_proto.h>
#include <mlib_graphics.h>
#include <mlib_GraphicsDrawSet.h>

/* *********************************************************** */

#define	MLIB_LOAD_XZ(MLIB_TYPE)                                 \
	y = coords[MLIB_GDS_STARTY];                            \
	xs = coords[MLIB_GDS_STARTX];                           \
	xf = coords[MLIB_GDS_STOPX];                            \
	data = (MLIB_TYPE *) (rtable[y]);                       \
	zdata = (mlib_s16 *)(zrtable[y])

/* *********************************************************** */

#define	MLIB_LOAD_GZ_8                                          \
	zdata += xs;                                            \
	z = coords[MLIB_GDS_STARTZ];                            \
	dz = coords[MLIB_GDS_STEPZ];                            \
	c = coords[MLIB_GDS_STARTC];                            \
	dc = coords[MLIB_GDS_STEPC];                            \
	zz = *zdata;                                            \
	stz = z >> MLIB_SHIFT;                                  \
	z += dz

/* *********************************************************** */

#define	MLIB_CYCLE_GZ_8                                         \
	for (j = xs; j < xf; j++, z += dz)                      \
	    if ((stz) > zz) {                                   \
		zz = zdata[1];                                  \
		zdata++;                                        \
		stz = z >> MLIB_SHIFT;                          \
		c += dc;                                        \
	    } else {                                            \
		zz = zdata[1];                                  \
		*zdata = stz;                                   \
		zdata++;                                        \
		stz = z >> MLIB_SHIFT;                          \
		data[j] = c >> MLIB_SHIFT;                      \
		c += dc;                                        \
	    }

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

	if (((mlib_addr)rtable[0] | (mlib_addr)zrtable[0] | (mlib_addr)rtable[1]
	    | (mlib_addr)zrtable[1]) & 7)
#pragma pipeloop(0)
		for (; coords < coordsend; coords += MLIB_GDS_NEXT_GZ) {
			MLIB_LOAD_XZ(mlib_u8);

			MLIB_LOAD_GZ_8;

#pragma pipeloop(0)
			MLIB_CYCLE_GZ_8;
	} else {
		vis_write_gsr(0);

		for (; coords < coordsend; coords += MLIB_GDS_NEXT_GZ) {
			MLIB_LOAD_XZ(mlib_u8);

			if (xf - xs < 8) {
				MLIB_LOAD_GZ_8;

#pragma pipeloop(0)
				MLIB_CYCLE_GZ_8;
			} else {
				mlib_s32 emask = 0xFF, xsa = xs & ~7;
				mlib_s32 emask0, emask1;
				mlib_d64 *pwz = (mlib_d64 *)(zdata + xsa);
				mlib_d64 w_z0, w_z1, w_stz0, w_stz1;
				mlib_d64 w_sz, w_tz0, w_tz1, w_tz2, w_tz3;
				mlib_d64 w_c, w_dc, w_c1, w_stc;
				mlib_s32 tc0, tc1, tc2, tc3;

				z = coords[MLIB_GDS_STARTZ] << (16 -
				    MLIB_SHIFT);
				dz = coords[MLIB_GDS_STEPZ] << (16 -
				    MLIB_SHIFT);
				c = coords[MLIB_GDS_STARTC];
				dc = coords[MLIB_GDS_STEPC];
				w_z0 = pwz[0];
				w_z1 = pwz[1];
				z -= (xs - xsa) * dz;
				c -= (xs - xsa) * dc;

				tc0 = (c << (23 - MLIB_SHIFT)) & ~0xFFFF;
				tc1 = (c + dc) >> (MLIB_SHIFT - 7);
				tc2 =
				    ((c + 2 * dc) << (23 -
				    MLIB_SHIFT)) & ~0xFFFF;
				tc3 = (c + 3 * dc) >> (MLIB_SHIFT - 7);
				w_c = vis_to_double(tc0 | tc1, tc2 | tc3);
				tc0 = dc >> (MLIB_SHIFT - 9);
				w_dc =
				    vis_to_double_dup((tc0 & 0xFFFF) | (tc0 <<
				    16));
				w_c1 = vis_fpadd16(w_c, w_dc);

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

#pragma pipeloop(0)
#pragma unroll(1)
				for (j = xsa; j < xf - 7; j += 8) {
					w_stc = vis_fpack16_pair(w_c, w_c1);
					w_tz0 = vis_fpadd32(w_tz0, w_sz);
					w_tz1 = vis_fpadd32(w_tz1, w_sz);
					w_tz2 = vis_fpadd32(w_tz2, w_sz);
					w_tz3 = vis_fpadd32(w_tz3, w_sz);
					w_c = vis_fpadd16(w_c1, w_dc);
					w_c1 = vis_fpadd16(w_c, w_dc);
					w_z0 = pwz[2];
					w_z1 = pwz[3];
					vis_pst_16(w_stz0, pwz++, emask0);
					vis_pst_16(w_stz1, pwz++, emask1);
					vis_pst_8(w_stc, data + j,
					    (emask0 << 4) | emask1);
					w_stz0 =
					    vis_fpackfix_pair(w_tz0, w_tz1);
					w_stz1 =
					    vis_fpackfix_pair(w_tz2, w_tz3);
					emask0 = vis_fcmple16(w_stz0, w_z0);
					emask1 = vis_fcmple16(w_stz1, w_z1);
				}

				if (j < xf) {
					w_stc = vis_fpack16_pair(w_c, w_c1);
					emask = (emask0 << 4) | emask1;
					emask &= (0xFF << (8 - xf + j));
					emask0 = emask >> 4;
					vis_pst_16(w_stz0, pwz++, emask0);
					vis_pst_16(w_stz1, pwz, emask);
					vis_pst_8(w_stc, data + j, emask);
				}
			}
		}
	}
}

/* *********************************************************** */

#define	MLIB_LOAD_GZ_32                                               \
	zdata += xs;                                                  \
	z = coords[MLIB_GDS_STARTZ];                                  \
	dz = coords[MLIB_GDS_STEPZ];                                  \
	c2 = coords[MLIB_GDS_STARTC2] | (0xFF << (MLIB_SHIFT + 8));   \
	dc2 = coords[MLIB_GDS_STEPC2];                                \
	c1 = coords[MLIB_GDS_STARTC1];                                \
	dc1 = coords[MLIB_GDS_STEPC1];                                \
	c = coords[MLIB_GDS_STARTC];                                  \
	dc = coords[MLIB_GDS_STEPC];                                  \
	zz = *zdata;                                                  \
	stz = z >> MLIB_SHIFT;                                        \
	z += dz

/* *********************************************************** */

#define	MLIB_CYCLE_GZ_32                                        \
	for (j = xs; j < xf; j++, z += dz)                      \
	    if ((stz) > zz) {                                   \
		zz = zdata[1];                                  \
		zdata++;                                        \
		stz = z >> MLIB_SHIFT;                          \
		c2 += dc2;                                      \
		c1 += dc1;                                      \
		c += dc;                                        \
	    } else {                                            \
		mlib_u8 *data1 = data + 4 * j;                  \
	                                                        \
		zz = zdata[1];                                  \
		*zdata = stz;                                   \
		zdata++;                                        \
		stz = z >> MLIB_SHIFT;                          \
		*(mlib_s16 *)data1 = c2 >> MLIB_SHIFT;          \
		c2 += dc2;                                      \
		data1[2] = c1 >> MLIB_SHIFT;                    \
		c1 += dc1;                                      \
		data1[3] = c >> MLIB_SHIFT;                     \
		c += dc;                                        \
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
	mlib_f32 v_zero = vis_to_float(0);
	mlib_f32 v_7f = vis_to_float(0x7FFFFFFF);

	if (((mlib_addr)rtable[0] | (mlib_addr)zrtable[0] | (mlib_addr)rtable[1]
	    | (mlib_addr)zrtable[1]) & 7)
#pragma pipeloop(0)
		for (; coords < coordsend; coords += MLIB_GDS_NEXT_GZ32) {
			MLIB_LOAD_XZ(mlib_u8);

			MLIB_LOAD_GZ_32;

#pragma pipeloop(0)
			MLIB_CYCLE_GZ_32;
	} else {
#pragma pipeloop(0)
		for (; coords < coordsend; coords += MLIB_GDS_NEXT_GZ32) {
			MLIB_LOAD_XZ(mlib_u8);

			if (xf - xs < 8) {
				MLIB_LOAD_GZ_32;

#pragma pipeloop(0)
				MLIB_CYCLE_GZ_32;
			} else {
				mlib_s32 xsa = xs & ~3;
				mlib_s32 emask0 = 0xF;
				mlib_d64 *pwz = (mlib_d64 *)(zdata + xsa);
				mlib_d64 *pwx = (mlib_d64 *)(data + 4 * xsa);
				mlib_d64 w_z0, w_stz0;
				mlib_d64 w_sz, w_tz0, w_tz1;
				mlib_d64 w_c, w_c2, w_dc, w_dc2, w_stc;

				z = 2 * coords[MLIB_GDS_STARTZ];
				dz = 2 * coords[MLIB_GDS_STEPZ];
				w_z0 = pwz[0];

				vis_write_gsr(8 << 3);
				w_c2 =
				    vis_freg_pair(v_7f,
				    ((mlib_f32 *)coords)[MLIB_GDS_STARTC2]);
				w_c =
				    vis_freg_pair(((mlib_f32 *)
				    coords)[MLIB_GDS_STARTC1],
				    ((mlib_f32 *)coords)[MLIB_GDS_STARTC]);
				w_dc2 =
				    vis_freg_pair(v_zero,
				    ((mlib_f32 *)coords)[MLIB_GDS_STEPC2]);
				w_dc =
				    vis_freg_pair(((mlib_f32 *)
				    coords)[MLIB_GDS_STEPC1],
				    ((mlib_f32 *)coords)[MLIB_GDS_STEPC]);
				w_c = vis_fpackfix_pair(w_c2, w_c);
				w_dc = vis_fpackfix_pair(w_dc2, w_dc);
				vis_write_gsr(0);

				for (j = 0; j < xs - xsa; j++, z -= dz)
					w_c = vis_fpsub16(w_c, w_dc);

				w_c2 = vis_fpadd16(w_c, w_dc);
				w_dc = vis_fpadd16(w_dc, w_dc);

				emask0 = emask0 >> (xs - xsa);
				w_tz0 = vis_to_double(z, z + dz);
				w_sz = vis_to_double_dup(dz + dz);
				w_tz1 = vis_fpadd32(w_tz0, w_sz);
				w_sz = vis_fpadd32(w_sz, w_sz);

				w_stz0 = vis_fpackfix_pair(w_tz0, w_tz1);
				emask0 &= vis_fcmple16(w_stz0, w_z0);

#pragma unroll(1)
				for (j = xsa; j < xf - 3; j += 4) {
					w_tz0 = vis_fpadd32(w_tz0, w_sz);
					w_tz1 = vis_fpadd32(w_tz1, w_sz);
					w_z0 = pwz[1];
					vis_pst_16(w_stz0, pwz++, emask0);
					w_stc = vis_fpack16_pair(w_c, w_c2);
					w_c = vis_fpadd16(w_c, w_dc);
					w_c2 = vis_fpadd16(w_c2, w_dc);
					vis_pst_32(w_stc, pwx++, emask0 >> 2);
					w_stc = vis_fpack16_pair(w_c, w_c2);
					w_c = vis_fpadd16(w_c, w_dc);
					w_c2 = vis_fpadd16(w_c2, w_dc);
					vis_pst_32(w_stc, pwx++, emask0);
					w_stz0 =
					    vis_fpackfix_pair(w_tz0, w_tz1);
					emask0 = vis_fcmple16(w_stz0, w_z0);
				}

				if (j < xf) {
					emask0 &= (0xF << (4 - xf + j));
					vis_pst_16(w_stz0, pwz, emask0);
					w_stc = vis_fpack16_pair(w_c, w_c2);
					w_c = vis_fpadd16(w_c, w_dc);
					w_c2 = vis_fpadd16(w_c2, w_dc);
					vis_pst_32(w_stc, pwx++, emask0 >> 2);
					w_stc = vis_fpack16_pair(w_c, w_c2);
					vis_pst_32(w_stc, pwx, emask0);
				}
			}
		}
	}
}

/* *********************************************************** */
