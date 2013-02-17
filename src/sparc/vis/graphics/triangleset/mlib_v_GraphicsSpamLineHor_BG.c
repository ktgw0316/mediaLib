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

#pragma ident	"@(#)mlib_v_GraphicsSpamLineHor_BG.c	9.2	07/11/05 SMI"

/*
 *  FUNCTIONS
 *    mlib_GraphicsSpamLineHor_BG_8  - line filling function for mlib_u8 image
 *    mlib_GraphicsSpamLineHor_BG_32 - line filling function for mlib_s32 image
 *
 *  SYNOPSIS
 *    void  mlib_GraphicsSpamLineHor_BG_[8 | 32]
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
    mlib_s32 a)
{
	mlib_u8 *data, *data1, *data2, *dataend;
	mlib_s32 y, xs, xf, c, dc;
	mlib_d64 A0, A1;
	mlib_s32 sa, sonema, tail;
	mlib_f32 fa, ld, st, fmask;
	mlib_d64 donema, m0, m1, sum;

	A1 = (a & 0xFF) / 255.;
	A0 = 1. - A1;

	sa = a & 0xFF;
	sonema = ~a & 0xFF;
	sa = (sa << 24) | (sa << 16) | (sa << 8) | sa;
	fa = vis_to_float(sa);
/* 0xFF - alpha */
	donema = vis_to_double_dup((sonema << 23) | (sonema << 7));

	vis_write_gsr(0);

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
		tail = (mlib_addr)dataend & 3;
		data2 = dataend - tail;
		if (data1 < data2) {
			mlib_d64 w_c, w_dc;
			mlib_s32 tc0, tc1, tc2, tc3;

			for (; data < data1; data++) {
				MLIB_BLENDING_8(data[0], (c >> MLIB_SHIFT));
				c += dc;
			}

			tc0 = (c << (23 - MLIB_SHIFT)) & ~0xFFFF;
			tc1 = (c + dc) >> (MLIB_SHIFT - 7);
			tc2 = ((c + 2 * dc) << (23 - MLIB_SHIFT)) & ~0xFFFF;
			tc3 = (c + 3 * dc) >> (MLIB_SHIFT - 7);
			w_c = vis_to_double(tc0 | tc1, tc2 | tc3);
			tc0 = dc >> (MLIB_SHIFT - 9);
			w_dc = vis_to_double_dup((tc0 & 0xFFFF) | (tc0 << 16));

			for (; data < data2; data += 4) {
				ld = *(mlib_f32 *)data;
				m0 = vis_fmul8x16(ld, donema);
				m1 = vis_fmul8x16(fa, w_c);
				sum = vis_fpadd16(m0, m1);
				st = vis_fpack16(sum);
				*(mlib_f32 *)data = st;
				w_c = vis_fpadd16(w_c, w_dc);
			}

			if (tail) {
				fmask =
				    vis_to_float(0xFFFFFFFF << ((4 -
				    tail) << 3));
				ld = *(mlib_f32 *)data;
				m0 = vis_fmul8x16(ld, donema);
				m1 = vis_fmul8x16(fa, w_c);
				sum = vis_fpadd16(m0, m1);
				st = vis_fpack16(sum);
				*(mlib_f32 *)data = vis_fands(st, fmask);
			}
		} else {
#pragma unroll(1)
			for (; data < dataend; data++) {
				MLIB_BLENDING_8(data[0], (c >> MLIB_SHIFT));
				c += dc;
			}
		}
	}
}

/* *********************************************************** */

#define	MLIB_PUT_BG_32(DATA, X)                                 \
	ld = *(mlib_f32 *)(DATA + X);                           \
	m0 = vis_fmul8x16(ld, donema);                          \
	m1 = vis_fmul8x16(fa, w_c);                             \
	sum = vis_fpadd16(m0, m1);                              \
	st = vis_fpack16(sum);                                  \
	st = vis_fors(st, falpha);                              \
	*(mlib_f32 *)(DATA + X) = st;                           \
	w_c = vis_fpadd16(w_c, w_dc);

/* *********************************************************** */

void
mlib_GraphicsSpamLineHor_BG_32(
    mlib_u8 **rtable,
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 a)
{
	mlib_d64 w_c, w_c2, w_dc, w_dc2;
	mlib_f32 v_zero = vis_to_float(0);
	mlib_f32 v_7f = vis_to_float(0x7FFFFFFF);
	mlib_u8 *data;
	mlib_s32 j, y, xs, xf;
	mlib_s32 sa, sonema;
	mlib_f32 fa, ld;
	mlib_d64 donema, m0, m1, sum, sum0, sum1, dld;
	mlib_f32 falpha = vis_to_float(0xFF000000), st;
	mlib_d64 dalpha = vis_to_double_dup(0xFF000000), dst;

	sa = a & 0xFF;
	sonema = ~a & 0xFF;
	sa = (sa << 16) | (sa << 8) | sa;
	fa = vis_to_float(sa);
/* 0xFF - alpha */
	donema = vis_to_double((sonema << 7), (sonema << 23) | (sonema << 7));

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
		j = xs;

		if (((mlib_addr)(data + (4 * j)) & 7) && (j < xf)) {
			MLIB_PUT_BG_32(data, (4 * j));
			j++;
		}

		for (; j < xf - 1; j += 2) {
			dld = *(mlib_d64 *)(data + (4 * j));
			m0 = vis_fmul8x16(vis_read_hi(dld), donema);
			m1 = vis_fmul8x16(fa, w_c);
			w_c = vis_fpadd16(w_c, w_dc);
			sum0 = vis_fpadd16(m0, m1);
			m0 = vis_fmul8x16(vis_read_lo(dld), donema);
			m1 = vis_fmul8x16(fa, w_c);
			w_c = vis_fpadd16(w_c, w_dc);
			sum1 = vis_fpadd16(m0, m1);
			dst = vis_fpack16_pair(sum0, sum1);
			dst = vis_for(dst, dalpha);
			*(mlib_d64 *)(data + (4 * j)) = dst;
		}

		if (j < xf) {
			MLIB_PUT_BG_32(data, (4 * j));
		}
	}
}

/* *********************************************************** */
